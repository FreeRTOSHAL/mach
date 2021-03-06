#ifndef IOMUX_H_
#define IOMUX_H_
enum pins{
	PTA0 = 0,
	PTA1 = 1,
	PTA2 = 2,
	PTA3 = 3,
	PTA4 = 4,
	PTA5 = 5,
	PTA6 = 6,
	PTA7 = 7,
	PTA8 = 8,
	PTA9 = 9,
	PTA10 = 10,
#if defined(CONFIG_S32K118) || defined(CONFIG_S32K142) || defined(CONFIG_S32K144) || defined(CONFIG_S32K146) || defined(CONFIG_S32K148)
	PTA11 = 11,
# if defined(CONFIG_S32K142) || defined(CONFIG_S32K144) || defined(CONFIG_S32K146) || defined(CONFIG_S32K148)
	PTA12 = 12,
	PTA13 = 13,
	PTA14 = 14,
	PTA15 = 15,
	PTA16 = 16,
	PTA17 = 17,
#  if defined(CONFIG_S32K146) || defined(CONFIG_S32K148)
	PTA18 = 18,
	PTA19 = 19,
	PTA20 = 20,
	PTA21 = 21,
	PTA22 = 22,
	PTA23 = 23,
	PTA24 = 24,
#   if defined(CONFIG_S32K148)
	PTA25 = 25,
	PTA26 = 26,
	PTA27 = 27,
	PTA28 = 28,
	PTA29 = 29,
	PTA30 = 30,
	PTA31 = 31,
#   endif
#  endif
# endif
#endif
	PTB0 = 32,
	PTB1 = 33,
	PTB2 = 34,
	PTB3 = 35,
	PTB4 = 36,
	PTB5 = 37,
	PTB6 = 38,
	PTB7 = 39,
	PTB8 = 40,
#if defined(CONFIG_S32K118) || defined(CONFIG_S32K142) || defined(CONFIG_S32K144) || defined(CONFIG_S32K146) || defined(CONFIG_S32K148)
	PTB9 = 41,
# if defined(CONFIG_S32K142) || defined(CONFIG_S32K144) || defined(CONFIG_S32K146) || defined(CONFIG_S32K148)
	PTB10 = 42,
	PTB11 = 43,
	PTB12 = 44,
	PTB13 = 45,
	PTB14 = 46,
	PTB15 = 47,
	PTB16 = 48,
	PTB17 = 49,
#  if defined(CONFIG_S32K146) || defined(CONFIG_S32K148)
	PTB18 = 50,
	PTB19 = 51,
	PTB20 = 52,
	PTB21 = 53,
	PTB22 = 54,
	PTB23 = 55,
	PTB24 = 56,
	PTB25 = 57,
	PTB26 = 58,
#   if defined(CONFIG_S32K148)
	PTB27 = 59,
	PTB28 = 60,
	PTB29 = 61,
	PTB30 = 62,
	PTB31 = 63,
#   endif
#  endif
# endif
#endif
	PTC0 = 64,
	PTC1 = 65,
	PTC2 = 66,
	PTC3 = 67,
	PTC4 = 68,
	PTC5 = 69,
	PTC6 = 70,
	PTC7 = 71,
	PTC8 = 72,
	PTC9 = 73,
	PTC10 = 74,
	PTC11 = 75,
#if defined(CONFIG_S32K118) || defined(CONFIG_S32K142) || defined(CONFIG_S32K144) || defined(CONFIG_S32K146) || defined(CONFIG_S32K148)
	PTC12 = 76,
	PTC13 = 77,
# if defined(CONFIG_S32K142) || defined(CONFIG_S32K144) || defined(CONFIG_S32K146) || defined(CONFIG_S32K148)
	PTC14 = 78,
	PTC15 = 79,
	PTC16 = 80,
	PTC17 = 81,
#  if defined(CONFIG_S32K146) || defined(CONFIG_S32K148)
	PTC18 = 82,
	PTC19 = 83,
	PTC20 = 84,
	PTC21 = 85,
	PTC22 = 86,
	PTC23 = 87,
	PTC24 = 88,
#   if defined(CONFIG_S32K148)
	PTC25 = 89,
	PTC26 = 90,
	PTC27 = 91,
	PTC28 = 92,
	PTC29 = 93,
	PTC30 = 94,
	PTC31 = 95,
#   endif
#  endif
# endif
#endif
	PTD0 = 96,
	PTD1 = 97,
	PTD2 = 98,
	PTD3 = 99,
	PTD4 = 100,
	PTD5 = 101,
	PTD6 = 102,
#if defined(CONFIG_S32K118) || defined(CONFIG_S32K142) || defined(CONFIG_S32K144) || defined(CONFIG_S32K146) || defined(CONFIG_S32K148)
	PTD7 = 103,
	PTD8 = 104,
	PTD9 = 105,
# if defined(CONFIG_S32K142) || defined(CONFIG_S32K144) || defined(CONFIG_S32K146) || defined(CONFIG_S32K148)
	PTD10 = 106,
	PTD11 = 107,
	PTD12 = 108,
	PTD13 = 109,
	PTD14 = 110,
	PTD15 = 111,
	PTD16 = 112,
	PTD17 = 113,
#  if defined(CONFIG_S32K146) || defined(CONFIG_S32K148)
	PTD18 = 114,
	PTD19 = 115,
	PTD20 = 116,
	PTD21 = 117,
	PTD22 = 118,
	PTD23 = 119,
	PTD24 = 120,
	PTD25 = 121,
	PTD26 = 122,
#   if defined(CONFIG_S32K148)
	PTD27 = 123,
	PTD28 = 124,
	PTD29 = 125,
	PTD30 = 126,
	PTD31 = 127,
#   endif
#  endif
# endif
#endif
	PTE0 = 128,
	PTE1 = 129,
	PTE2 = 130,
	PTE3 = 131,
#if defined(CONFIG_S32K118) || defined(CONFIG_S32K142) || defined(CONFIG_S32K144) || defined(CONFIG_S32K146) || defined(CONFIG_S32K148)
	PTE4 = 132,
	PTE5 = 133,
	PTE6 = 134,
	PTE7 = 135,
	PTE8 = 136,
	PTE9 = 137,
	PTE10 = 138,
	PTE11 = 139,
# if defined(CONFIG_S32K142) || defined(CONFIG_S32K144) || defined(CONFIG_S32K146) || defined(CONFIG_S32K148)
	PTE12 = 140,
	PTE13 = 141,
	PTE14 = 142,
	PTE15 = 143,
	PTE16 = 144,
#  if defined(CONFIG_S32K146) || defined(CONFIG_S32K148)
	PTE17 = 145,
	PTE18 = 146,
	PTE19 = 147,
	PTE20 = 148,
	PTE21 = 149,
	PTE22 = 150,
	PTE23 = 151,
#   if defined(CONFIG_S32K148)
	PTE24 = 152,
	PTE25 = 153,
	PTE26 = 154,
	PTE27 = 155,
#   endif
#  endif
# endif
#endif
};
#endif
