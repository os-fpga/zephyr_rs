/*
 * Copyright (c) 2024 Rapid Silicon
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef ZEPHYR_DRIVERS_CRYPTO_PUFS_H_
#define ZEPHYR_DRIVERS_CRYPTO_PUFS_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdint.h>
#include <zephyr/crypto/crypto.h>

/*** Generic PUFcc defines ***/
#define PUFCC_WORD_SIZE 4
#define PUFCC_BUSY_BIT_MASK 0x00000001

/*** RT and OTP defines ***/
#define PUFCC_RT_OFFSET 0x3000
#define PUFCC_RT_OTP_OFFSET 0x400
#define PUFCC_RT_ERROR_MASK 0x0000001e
#define PUFCC_OTP_LEN 1024
#define PUFCC_OTP_KEY_LEN 32
#define PUFCC_OTP_ZEROIZE_BASE_CMD 0x80

// One read/write lock register controls 8 OTP words
#define PUFCC_OTP_WORDS_PER_RWLCK_REG 8

// 4 bits are reserved for lock value of one OTP word in read/write lock
// register
#define PUFCC_OTP_RWLCK_REG_BITS_PER_OTP_WORD 4
#define PUFCC_PIF_RWLCK_MASK 0xF
#define PUFCC_PIF_MAX_RWLOCK_REGS \
  (PUFCC_OTP_LEN / PUFCC_WORD_SIZE / PUFCC_OTP_WORDS_PER_RWLCK_REG)

// Start index of the RWLCK register in PIF registers group
#define PUFCC_PIF_RWLCK_START_INDEX 32

// Define all possible OTP lock values
#define PUFCC_OTP_RWLCK_RW_0 0x0  // Read Write access
#define PUFCC_OTP_RWLCK_RW_1 0x1  // Read Write access
#define PUFCC_OTP_RWLCK_RW_2 0x2  // Read Write access
#define PUFCC_OTP_RWLCK_RW_3 0x4  // Read Write access
#define PUFCC_OTP_RWLCK_RW_4 0x8  // Read Write access
#define PUFCC_OTP_RWLCK_RO_0 0x3  // Read Only access
#define PUFCC_OTP_RWLCK_RO_1 0x7  // Read Only access
#define PUFCC_OTP_RWLCK_RO_2 0xb  // Read Only access

/*** DMA defines ***/
#define PUFCC_DMA_KEY_DST_HASH 0x1
#define PUFCC_DMA_KEY_DST_SP38A 0x8
#define PUFCC_DMA_DSC_CFG2_SGDMA_VAL 0x20
#define PUFCC_DMA_ERROR_MASK 0xFFFFFFFE

/*** HMAC defines ***/
#define PUFCC_HMAC_OFFSET 0x0800
#define PUFCC_HMAC_SW_KEY_MAXLEN 64
#define PUFCC_SHA_256_LEN 32
#define PUFCC_HMAC_FUNCTION_HASH 0x0
#define PUFCC_HMAC_VARIANT_SHA256 0x03

/*** Crypto defines ***/
#define PUFCC_CRYPTO_OFFSET 0x0100
#define PUFCC_CRYPTO_SW_KEY_MAXLEN 64  // The byte length of CRYPTO_SW_KEY_ADDR
#define PUFCC_CRYPTO_DGST_LEN 64       // The byte length of CRYPTO_DGST field
#define PUFCC_CRYPTO_IV_MAXLEN 16
#define PUFCC_CRYPTO_AES128_KEY_LEN 16
#define PUFCC_CRYPTO_AES256_KEY_LEN 32

/*** SP38a defines ***/
#define PUFCC_SP38A_OFFSET 0x0200
#define PUFCC_SP38A_STATUS_ERROR_MASK 0xfffff0c0

/*** PKC defines ***/
#define PUFCC_PKC_OFFSET 0x1000
#define PUFCC_RSA_2048_LEN 256
#define PUFCC_ECDSA_256_LEN 32
#define PUFCC_DATA_RSA2048_MODULUS_OFFSET 256
#define PUFCC_DATA_RSA2048_SIGN_OFFSET 768
#define PUFCC_DATA_ECDSA_PRIME_OFFSET 256
#define PUFCC_PKC_ERROR_MASK 0xFFFFFFFE
#define PUFCC_DATA_ECDSA_EC_A_OFFSET \
  (PUFCC_DATA_ECDSA_PRIME_OFFSET + PUFCC_ECDSA_256_LEN)

#define PUFCC_DATA_ECDSA_EC_B_OFFSET \
  (PUFCC_DATA_ECDSA_EC_A_OFFSET + PUFCC_ECDSA_256_LEN)

#define PUFCC_DATA_ECDSA_PX_OFFSET \
  (PUFCC_DATA_ECDSA_EC_B_OFFSET + PUFCC_ECDSA_256_LEN)

#define PUFCC_DATA_ECDSA_PY_OFFSET \
  (PUFCC_DATA_ECDSA_PX_OFFSET + PUFCC_ECDSA_256_LEN)

#define PUFCC_DATA_ECDSA_ORDER_OFFSET \
  (PUFCC_DATA_ECDSA_PY_OFFSET + PUFCC_ECDSA_256_LEN)

#define PUFCC_DATA_ECDSA_HASH_OFFSET \
  (PUFCC_DATA_ECDSA_ORDER_OFFSET + PUFCC_ECDSA_256_LEN)

#define PUFCC_DATA_ECDSA_PUBX_OFFSET \
  (PUFCC_DATA_ECDSA_HASH_OFFSET + PUFCC_ECDSA_256_LEN)

#define PUFCC_DATA_ECDSA_PUBY_OFFSET \
  (PUFCC_DATA_ECDSA_PUBX_OFFSET + PUFCC_ECDSA_256_LEN)

#define PUFCC_DATA_ECDSA_SIG_R_OFFSET \
  (PUFCC_DATA_ECDSA_PUBY_OFFSET + PUFCC_ECDSA_256_LEN)

#define PUFCC_DATA_ECDSA_SIG_S_OFFSET \
  (PUFCC_DATA_ECDSA_SIG_R_OFFSET + PUFCC_ECDSA_256_LEN)

/**
 * @brief SHA lengths
 */
#define PUFS_SHA_MAX_LEN 64
#define PUFS_SHA256_LEN 32

/**
 * @brief ECDSA256 quadrant and key lengths
 */
#define PUFS_EC256_QLEN 32
#define PUFS_EC256_KEY_LEN (32 * 2)

/**
 * @brief RSA 2048 public key modulus length
 *
 */
#define PUFS_RSA_2048_LEN 256

/*****************************************************************************
 * Enumerations
 ****************************************************************************/
// PUFcc status codes
enum pufcc_status {
  PUFCC_SUCCESS,      // Success.
  PUFCC_E_ALIGN,      // Address alignment mismatch.
  PUFCC_E_OVERFLOW,   // Space overflow.
  PUFCC_E_UNDERFLOW,  // Size too small.
  PUFCC_E_INVALID,    // Invalid argument.
  PUFCC_E_BUSY,       // Resource is occupied.
  PUFCC_E_UNAVAIL,    // Resource is unavailable.
  PUFCC_E_FIRMWARE,   // Firmware error.
  PUFCC_E_VERFAIL,    // Invalid public key or digital signature.
  PUFCC_E_ECMPROG,    // Invalid ECC microprogram.
  PUFCC_E_DENY,       // Access denied.
  PUFCC_E_UNSUPPORT,  // Not support.
  PUFCC_E_INFINITY,   // Point at infinity.
  PUFCC_E_ERROR,      // Unspecific error.
  PUFCC_E_TIMEOUT,    // Operation timed out.
};

// PUFcc key slots; 32 slots of 256 bits each
enum pufcc_otp_slot {
  PUFCC_OTPKEY_0,   // OTP key slot 0, 256 bits
  PUFCC_OTPKEY_1,   // OTP key slot 1, 256 bits
  PUFCC_OTPKEY_2,   // OTP key slot 2, 256 bits
  PUFCC_OTPKEY_3,   // OTP key slot 3, 256 bits
  PUFCC_OTPKEY_4,   // OTP key slot 4, 256 bits
  PUFCC_OTPKEY_5,   // OTP key slot 5, 256 bits
  PUFCC_OTPKEY_6,   // OTP key slot 6, 256 bits
  PUFCC_OTPKEY_7,   // OTP key slot 7, 256 bits
  PUFCC_OTPKEY_8,   // OTP key slot 8, 256 bits
  PUFCC_OTPKEY_9,   // OTP key slot 9, 256 bits
  PUFCC_OTPKEY_10,  // OTP key slot 10, 256 bits
  PUFCC_OTPKEY_11,  // OTP key slot 11, 256 bits
  PUFCC_OTPKEY_12,  // OTP key slot 12, 256 bits
  PUFCC_OTPKEY_13,  // OTP key slot 13, 256 bits
  PUFCC_OTPKEY_14,  // OTP key slot 14, 256 bits
  PUFCC_OTPKEY_15,  // OTP key slot 15, 256 bits
  PUFCC_OTPKEY_16,  // OTP key slot 16, 256 bits
  PUFCC_OTPKEY_17,  // OTP key slot 17, 256 bits
  PUFCC_OTPKEY_18,  // OTP key slot 18, 256 bits
  PUFCC_OTPKEY_19,  // OTP key slot 19, 256 bits
  PUFCC_OTPKEY_20,  // OTP key slot 20, 256 bits
  PUFCC_OTPKEY_21,  // OTP key slot 21, 256 bits
  PUFCC_OTPKEY_22,  // OTP key slot 22, 256 bits
  PUFCC_OTPKEY_23,  // OTP key slot 23, 256 bits
  PUFCC_OTPKEY_24,  // OTP key slot 24, 256 bits
  PUFCC_OTPKEY_25,  // OTP key slot 25, 256 bits
  PUFCC_OTPKEY_26,  // OTP key slot 26, 256 bits
  PUFCC_OTPKEY_27,  // OTP key slot 27, 256 bits
  PUFCC_OTPKEY_28,  // OTP key slot 28, 256 bits
  PUFCC_OTPKEY_29,  // OTP key slot 29, 256 bits
  PUFCC_OTPKEY_30,  // OTP key slot 30, 256 bits
  PUFCC_OTPKEY_31,  // OTP key slot 31, 256 bits
};

// OTP lock types
enum pufcc_otp_lock {
  PUFCC_OTP_NA = 0xF,  // No-Access
  PUFCC_OTP_RO = 0x3,  // Read-Only
  PUFCC_OTP_RW = 0x0,  // Read-Write
};

// PUFcc read/write types
enum pufcc_dma_rw_type { AUTO_INCREMENT, FIXED_RW };

// PUFcc key types
enum pufcc_key_type { PUFCC_SW_KEY, PUFCC_OTP_KEY };

// PUFcc SP38a variants
enum pufcc_sp38a_variant {
  PUFCC_AES128,
  PUFCC_AES192,
  PUFCC_AES256,
  PUFCC_SM4
};

// PUFcc PKC schemes
enum pufcc_pkc_scheme { PUFCC_RSA_2048 = 0x86, PUFCC_ECDSA256 = 0x82 };

// PUFcc SP38a modes
enum pufcc_sp38a_mode {
  PUFCC_ECB_CLR,
  PUFCC_CFB,
  PUFCC_OFB,
  PUFCC_CBC_CLR,
  PUFCC_CBC_CTS1,
  PUFCC_CBC_CTS2,
  PUFCC_CBC_CTS3,
  PUFCC_CTR32,
  PUFCC_CTR64,
  PUFCC_CTR128
};

/**
 * @enum  pufs_crypto_tfr_type
 * @brief Types of secure transfer type in case of peripherals
 */
enum pufs_crypto_tfr_type {
  PUFS_SECURE_TX,  // Write to peripheral
  PUFS_SECURE_RX   // Read from peripheral
};

enum pufs_session_type {
  PUFS_SESSION_SIGN_VERIFICATION = 0,
  PUFS_SESSION_HASH_CALCULATION,
  PUFS_SESSION_DECRYPTION,
  PUFS_SESSION_UNDEFINED
};

// Scatter gather DMA descriptor struct
struct pufcc_sg_dma_desc {
  uint32_t read_addr;
  uint32_t write_addr;
  uint32_t length;
  uint32_t next;
  uint32_t dsc_cfg_4;
  uint32_t key_cfg;
  uint32_t cypt_cfg[2];
};

/*****************************************************************************
 * Register structures
 ****************************************************************************/
// General register structs
struct pufcc_intrpt_reg {
  uint32_t intrpt_st : 1;
  uint32_t reserved1 : 15;
  uint32_t intrpt_en : 1;
  uint32_t reserved2 : 15;
};

struct pufcc_start_reg {
  uint32_t start_p : 1;
  uint32_t reserved : 31;
};

// DMA register structs
struct pufcc_dma_cfg_0_reg {
  uint32_t rng_en : 1;
  uint32_t sg_en : 1;
  uint32_t reserved : 30;
};

struct pufcc_dma_cfg_1_reg {
  uint8_t rbst_max;
  uint8_t wbst_max;
  uint8_t rbst_min;
  uint8_t wbst_min;
};

struct pufcc_dma_dsc_cfg_4_reg {
  uint32_t wprot : 8;
  uint32_t rprot : 8;
  uint32_t fw : 1;
  uint32_t fr : 1;
  uint32_t reserved : 5;
  uint32_t no_cypt : 1;
  uint32_t offset : 4;
  uint32_t dn_pause : 1;
  uint32_t dn_intrpt : 1;
  uint32_t tail : 1;
  uint32_t head : 1;
};

struct pufcc_dma_key_cfg0_reg {
  uint32_t key_src : 4;
  uint32_t key_dst : 4;
  uint32_t key_size : 11;
  uint32_t reserved1 : 5;
  uint32_t key_idx : 5;
  uint32_t reserved2 : 3;
};

// HMAC register structs
struct pufcc_hmac_config_reg {
  uint32_t variant : 4;
  uint32_t reserved : 4;
  uint32_t function : 1;
  uint32_t reserved2 : 23;
};

// PUFcc SP38a register structs
struct pufcc_sp38a_config_reg {
  uint32_t variant : 2;
  uint32_t reserved1 : 2;
  uint32_t mode : 4;
  uint32_t enc_dec : 1;
  uint32_t reserved2 : 23;
};

// PKC register register structs
struct pufcc_pkc_ecp_ec_reg {
  uint32_t reserved1 : 8;
  uint32_t field : 8;  // PKC scheme
  uint32_t h : 4;      // EC cofactor h
  uint32_t reserved2 : 12;
};

// ECC parameters structure
struct pufcc_ecc_param {
  const void *prime;  // Field modulus
  const void *a;      // EC parameter a
  const void *b;      // EC parameter b
  const void *px;     // x-coordinate of base point P
  const void *py;     // y-coordinate of base point P
  const void *order;  // Subgroup order
};

/*****************************************************************************
 * PUFcc register maps
 ****************************************************************************/
// OTP memory map
struct pufcc_otp_mem {
  uint32_t otp[256];
};

struct pufcc_rt_regs {
  volatile uint32_t pif[64];
  uint32_t _pad1[64];
  volatile uint32_t ptr[16];
  volatile uint32_t ptc[16];
  volatile uint32_t ptm[2];
  uint32_t _pad2[6];
  volatile uint32_t rn;
  volatile uint32_t rn_status;
  volatile uint32_t healthcfg;
  volatile uint32_t feature;
  volatile uint32_t interrupt;
  volatile uint32_t otp_psmsk[2];
  volatile uint32_t puf_psmsk;
  volatile uint32_t version;
  volatile uint32_t status;
  volatile uint32_t cfg;
  volatile uint32_t set_pin;
  volatile uint32_t auto_repair;
  volatile uint32_t ini_off_chk;
  volatile uint32_t repair_pgn;
  volatile uint32_t repair_reg;
  volatile uint32_t puf_qty_chk;
  volatile uint32_t puf_enroll;
  volatile uint32_t puf_zeroize;
  volatile uint32_t set_flag;
  volatile uint32_t otp_zeroize;
  uint32_t _pad3[3];
  volatile uint32_t puf[64];
  volatile uint32_t otp[256];
};

// DMA module register map
struct pufcc_dma_regs {
  volatile uint32_t version;
  volatile uint32_t interrupt;
  volatile uint32_t feature;
  uint32_t _pad1;
  volatile uint32_t status_0;
  volatile uint32_t status_1;
  uint32_t _pad2[2];
  volatile uint32_t start;
  volatile uint32_t cfg_0;
  volatile uint32_t cfg_1;
  uint32_t _pad3[2];
  volatile uint32_t dsc_cfg_0;
  volatile uint32_t dsc_cfg_1;
  volatile uint32_t dsc_cfg_2;
  volatile uint32_t dsc_cfg_3;
  volatile uint32_t dsc_cfg_4;
  uint32_t _pad4[2];
  volatile uint32_t dsc_cur_0;
  volatile uint32_t dsc_cur_1;
  volatile uint32_t dsc_cur_2;
  volatile uint32_t dsc_cur_3;
  volatile uint32_t dsc_cur_4;
  uint32_t _pad5[2];
  volatile uint32_t key_cfg_0;
  volatile uint32_t cl_cfg_0;
};

// HMAC module register map
struct pufcc_hmac_regs {
  volatile uint32_t version;
  volatile uint32_t interrupt;
  volatile uint32_t feature;
  uint32_t _pad1;
  volatile uint32_t status;
  uint32_t _pad2;
  volatile uint32_t cfg;
  uint32_t _pad3;
  volatile uint32_t plen;
  uint32_t _pad4[3];
  volatile uint32_t alen;
  uint32_t _pad5[3];
  volatile uint8_t sw_key[PUFCC_HMAC_SW_KEY_MAXLEN];
};

// Crypto module register map
struct pufcc_crypto_regs {
  volatile uint32_t version;
  volatile uint32_t interrupt;
  volatile uint32_t feature;
  uint32_t _pad1[5];
  volatile uint32_t iv_out[PUFCC_CRYPTO_IV_MAXLEN / PUFCC_WORD_SIZE];
  volatile uint32_t iv[PUFCC_CRYPTO_IV_MAXLEN / PUFCC_WORD_SIZE];
  volatile uint32_t sw_key[PUFCC_CRYPTO_SW_KEY_MAXLEN / PUFCC_WORD_SIZE];
  volatile uint32_t dgst_in[PUFCC_CRYPTO_DGST_LEN / PUFCC_WORD_SIZE];
  volatile uint32_t dgst_out[PUFCC_CRYPTO_DGST_LEN / PUFCC_WORD_SIZE];
};

// SP38a module register map
struct pufcc_sp38a_regs {
  volatile uint32_t version;
  volatile uint32_t interrupt;
  volatile uint32_t feature;
  uint32_t _pad1;
  volatile uint32_t status;
  uint32_t _pad2;
  volatile uint32_t cfg;
};

// PKC module register map
struct pufcc_pkc_regs {
  volatile uint32_t version;
  volatile uint32_t interrupt;
  volatile uint32_t start;
  volatile uint32_t status;
  volatile uint32_t ecp_err_code;
  volatile uint32_t ecp_err_pc;
  volatile uint32_t ecp_err_cmd;
  volatile uint32_t mp_version;
  uint32_t _pad1[56];
  volatile uint32_t ecp_ec;
  volatile uint32_t ecp_keysel;
  volatile uint32_t ecp_otpkba;
  volatile uint32_t ecp_key_usage;
  volatile uint32_t ecp_e_short;
  uint32_t _pad2[55];
  volatile uint32_t ecp_mac[4];
  volatile uint32_t ecp_data[512];
};

/**
 * @struct pufs_crypto_addr
 * @brief  Address info for cryptographic operations
 */
struct pufs_crypto_addr {
  uint32_t read_addr;
  uint32_t write_addr;
  uint32_t len;
  enum pufs_crypto_tfr_type tfr_type;  // Transfer type (read or write) in case of
                                     // peripherals, otherwise don't care
  bool periph_rw;  // Indicates if data transfer involves a peripheral
  struct pufs_crypto_addr *next;  // In case data lies at multiple locations
};

/**
 * @struct pufs_crypto_rsa2048_puk
 * @brief  RSA 2048 public key structure
 */
struct pufs_crypto_rsa2048_puk {
  uint8_t n[PUFS_RSA_2048_LEN];  // Modulus
  uint32_t e;                  // Exponent
};

/**
 * @struct rs_crypto_ec256_puk
 * @brief  ECDSA256 public key
 */
struct rs_crypto_ec256_puk {
  uint8_t x[PUFS_EC256_QLEN];
  uint8_t y[PUFS_EC256_QLEN];
};

/**
 * @struct pufs_crypto_ec256_sig
 * @brief  ECDSA256 signature
 */
struct pufs_crypto_ec256_sig {
  uint8_t r[PUFS_EC256_QLEN];
  uint8_t s[PUFS_EC256_QLEN];
};

/**
 * @struct pufs_crypto_hash
 * @brief  Hash structure
 */
struct pufs_crypto_hash {
  uint8_t val[PUFS_SHA_MAX_LEN];
  uint32_t len;
};

/* Generic Callback Signature for different Sessions */
struct crypto_callbacks {
  	cipher_completion_cb cipher_cb;
    hash_completion_cb hash_cb;
    sign_completion_cb sign_cb;
};

/* Cipher, Hash and Sign session contexts */
struct pufs_crypto_ctx {
  struct hash_ctx *hash_ctx;
  struct cipher_ctx *cipher_ctx;
  struct sign_ctx *sign_ctx;
};

/* Cipher, Hash and Sign pkts */
struct pufs_crypto_pkt {
  struct hash_pkt *hash_pkt;
  struct cipher_pkt *cipher_pkt;
  struct sign_pkt *sign_pkt;
};

/* Device constant configuration parameters */
struct pufs_data {
	enum pufs_session_type pufs_session_type;
  struct crypto_callbacks session_callback;
  struct pufs_crypto_ctx pufs_ctx;
  struct pufs_crypto_pkt pufs_pkt;
};

/* Device constant configuration parameters */
struct pufs_config {
	void (*irq_init)(void);
	uint32_t base;
	uint32_t irq_num;
  uint32_t irq_priority;
  const struct device *dev;
};

/*
The following macro can be used to write register values.
REG_WRITE_32(Destination_Addr, Source_Addr)
*/
#define REG_WRITE_32(Destination_Addr, Source_Addr) \
  *(uint32_t *)Destination_Addr = *(uint32_t *)Source_Addr;

#endif /* ZEPHYR_DRIVERS_CRYPTO_CRYPTO_STM32_PRIV_H_ */
