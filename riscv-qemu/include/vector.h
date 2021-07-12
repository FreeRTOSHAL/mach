/* SPDX-License-Identifier: MIT */
/*
 * Author: Andreas Werner <kernel@andy89.org>
 * Date: 2021
 */
#ifndef VECTOR_H_
#define VECTOR_H_
typedef enum {
	VIRTIO0_IRQn = 0,
	VIRTIO1_IRQn = 1,
	VIRTIO2_IRQn = 2,
	VIRTIO3_IRQn = 3,
	VIRTIO4_IRQn = 4,
	VIRTIO5_IRQn = 5,
	VIRTIO6_IRQn = 6,
	VIRTIO7_IRQn = 7,
	RESERVED0_IRQn = 8,
	RESERVED1_IRQn = 9,
	UART0_IRQn = 10,
	RTC_IRQn = 11,
	RESERVED2_IRQn = 12,
	RESERVED3_IRQn = 13,
	RESERVED4_IRQn = 14,
	RESERVED5_IRQn = 15,
	RESERVED6_IRQn = 16,
	RESERVED7_IRQn = 17,
	RESERVED8_IRQn = 18,
	RESERVED9_IRQn = 19,
	RESERVED10_IRQn = 20,
	RESERVED11_IRQn = 21,
	RESERVED12_IRQn = 22,
	RESERVED13_IRQn = 23,
	RESERVED14_IRQn = 24,
	RESERVED15_IRQn = 25,
	RESERVED16_IRQn = 26,
	RESERVED17_IRQn = 27,
	RESERVED18_IRQn = 28,
	RESERVED19_IRQn = 29,
	RESERVED20_IRQn = 30,
	RESERVED21_IRQn = 31,
	PCIE0_IRQn = 32,
	PCIE1_IRQn = 33,
	PCIE2_IRQn = 34,
	PCIE3_IRQn = 35,
	RESERVED22_IRQn = 36,
	RESERVED23_IRQn = 37,
	RESERVED24_IRQn = 38,
	RESERVED25_IRQn = 39,
	RESERVED26_IRQn = 40,
	RESERVED27_IRQn = 41,
	RESERVED28_IRQn = 42,
	RESERVED29_IRQn = 43,
	RESERVED30_IRQn = 44,
	RESERVED31_IRQn = 45,
	RESERVED32_IRQn = 46,
	RESERVED33_IRQn = 47,
	RESERVED34_IRQn = 48,
	RESERVED35_IRQn = 49,
	RESERVED36_IRQn = 50,
	RESERVED37_IRQn = 51,
	RESERVED38_IRQn = 52,
	RESERVED39_IRQn = 53,
} IRQn_Type;
#endif