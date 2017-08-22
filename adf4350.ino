#include <SPI.h>

#define REF_CLK 10.0e6
#define PFD_FREQ 10.0e6

#define F(val, pos) (((uint32_t) val) << pos)

/* Register 0 */
#define r0 F(0, 0)
#define r0_integerValue(val) F(val, 15)
#define r0_fractionalValue(val) F(val, 3)

/* Register 1 */
#define r1 F(1, 0)
#define r1_prescaler(val) F(val, 27)
#define r1_phaseValue(val) F(val, 15)
#define r1_modulusValue(val) F(val, 3)

/* Register 2 */
#define r2 F(2, 0)
#define r2_lowNoiseAndLowSpurModes(val) F(val, 29)
#define r2_muxout(val) F(val, 26)
#define r2_referenceDoubler(val) F(val, 25)
#define r2_rdiv2(val) F(val, 24)
#define r2_rCounter(val) F(val, 14)
#define r2_dblBuf(val) F(val, 13)
#define r2_chargePumpCurrentSetting(val) F(val, 9)
#define r2_ldf(val) F(val, 8)
#define r2_ldp(val) F(val, 7)
#define r2_pdPolarity(val) F(val, 6)
#define r2_pd(val) F(val, 5)
#define r2_cpThreeState(val) F(val, 4)
#define r2_counterReset(val) F(val, 3)

/* Register 3 */
#define r3 F(3, 0)
#define r3_csr(val) F(val, 18)
#define r3_clkDivMode(val) F(val, 15)
#define r3_clockDividerValue(val) F(val, 3)

/* Register 4 */
#define r4 F(4, 0)
#define r4_feedbackSelect(val) F(val, 23)
#define r4_dividerSelect(val) F(val, 20)
#define r4_bandSelectClockDivider(val) F(val, 12)
#define r4_vcoPowerDown(val) F(val, 11)
#define r4_mtld(val) F(val, 10)
#define r4_auxOutputSelect(val) F(val, 9)
#define r4_auxOutputEnable(val) F(val, 8)
#define r4_auxOutputPower(val) F(val, 6)
#define r4_rfOutputEnable(val) F(val, 5)
#define r4_outputPower(val) F(val, 3)

/* Register 5 */
#define r5 (F(5, 0) | F(3, 19))
#define r5_ldPinMode(val) F(val, 22)

uint32_t transfer(int chip, int le, uint32_t data) {
  data = htonl(data);
  
  SPI.transfer(chip, &data, 4);
  
  digitalWrite(le, HIGH);
  delayMicroseconds(10);
  digitalWrite(le, LOW);
  delayMicroseconds(10);
  
  return ntohl(data);
}

uint32_t reg0 = r0;
uint32_t reg1 = r1;
uint32_t reg2 = r2 |
  r2_muxout(0) |
  r2_chargePumpCurrentSetting(7) |
  r2_ldp(1) |
  r2_pdPolarity(1);
uint32_t reg3 = r3 |
 r3_clkDivMode(0) |
 r3_clockDividerValue(200);
uint32_t reg4 = r4 |
  r4_feedbackSelect(1) |
  r4_rfOutputEnable(1) |
  r4_outputPower(0);
uint32_t reg5 = r5 |
  r5_ldPinMode(1);


int setFrequency(double frequency) {
  double VCO_frequency, divider, remainder, eps;
  int RF_DIV, INT, MOD, FRAC, PHASE, BS_CLK_DIV, PRESCALER, LDF;

  //BS_CLK_DIV    = (int)(ceil( PFD_FREQ / 125.0e3 )+1.0)*10;
  BS_CLK_DIV    = 200;
  PHASE     = 1;
  RF_DIV    = (int) ceil(log(2200.0e6 / frequency)/log(2));


  if (!(RF_DIV >= 0 && RF_DIV < 5)) {
    serprintf("RF_DIV < 0 || RF_DIV > 5: %d", RF_DIV);
    return 1;
  }

  VCO_frequency = frequency * pow(2.0, (double) RF_DIV);

  if (!(VCO_frequency >= 2200.0e6 && VCO_frequency <= 4400.0e6)) {
    serprintf("VCO_frequency >= 2200.0e6 && VCO_frequency <= 4400.0e6: %d\n", VCO_frequency);
  }

  divider     = VCO_frequency/PFD_FREQ;
  INT       = (int)floor(divider);

  if( VCO_frequency < 3000.0e6) {
    PRESCALER = 0;
    if (!(INT >= 23)) {
      serprintf("INT >= 23: %d", INT);
      return 1;
    }
  } else {
    PRESCALER = 1;
    if (!(INT >= 75)) {
      serprintf("INT >= 75: %d", INT);
      return 1;
    }
  }

  serprintf("Target frequency:.....%f MHz\n", frequency/1.0e6);
  serprintf("Target VCO frequency: %f MHz\n", VCO_frequency/1.0e6);

  remainder = divider - (double)INT;
  MOD   = 2;
  FRAC    = MOD-1;
  eps   = (fabs(remainder*(double)MOD - (double)FRAC));

  while (MOD < 4095 && eps > 1.0e-10) {
    MOD         += 1;
    FRAC  = (int)round(MOD * remainder);
    eps   = (fabs(remainder*(double)MOD - (double)FRAC));
  }


  if (!(FRAC >= 0 && FRAC < MOD)) {
    serprintf("FRAC >= 0 && FRAC < MOD: FRAC=%d, MOD=%d", FRAC, MOD);
    return 1;
  }
  if (!(MOD >= 2 && MOD <= 4095)) {
    serprintf("MOD >= 2 && MOD <= 4095: %d", MOD);
    return 1;
  }

  LDF   = (FRAC > 0) ? 0 : 1;

  serprintf("%f x %d = %d + %f = %d + %d/%d\n", divider, (int)pow(2.0, (double)RF_DIV),
            INT, remainder, INT, FRAC, MOD);

  serprintf("LDF = %d, BS_CLK_DIV = %d, RF_DIV = %d\n", LDF, BS_CLK_DIV, RF_DIV);

  transfer(4, 10, reg5);
  transfer(4, 10, reg4 | r4_bandSelectClockDivider(BS_CLK_DIV) | r4_dividerSelect(RF_DIV));
  transfer(4, 10, reg3);
  transfer(4, 10, reg2 | r2_rCounter(1) | r2_ldf(LDF));
  transfer(4, 10, reg1 | r1_prescaler(PRESCALER) | r1_phaseValue(PHASE) | r1_modulusValue(MOD));
  transfer(4, 10, reg0 | r0_integerValue(INT) | r0_fractionalValue(FRAC));

  return 0;

}

void begin() {
  // put your setup code here, to run once:
  SPI.begin(4);
  SPI.setClockDivider(4, 128); 
  SPI.setDataMode(4, SPI_MODE0);
  SPI.setBitOrder(4, MSBFIRST);
  pinMode(10, OUTPUT);
}
