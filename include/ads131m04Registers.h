/* Definitions of all the addresses of the registers of the ADS131M04
   Chip. For the content of the registers, please refer to the datasheet:

   https://www.ti.com/lit/gpn/ads131m04

   Written by Imperial College London Rocketry
   Electronics subteam
*/

#define ID 00h
#define STATUS 01h
#define MODE 02h
#define CLOCK 03h
#define GAIN1 04h
#define CFG 06h
#define THRSHLD_MSB 07h
#define THRSHLD_LSB 08h
#define CH0_CFG 09h
#define CH0_OCAL_MSB 0Ah
#define CH0_OCAL_LSB 0Bh
#define CH0_GCAL_MSB 0Ch
#define CH0_GCAL_LSB 0Dh
#define CH1_CFG 0Eh
#define CH1_OCAL_MSB 0Fh
#define CH1_OCAL_LSB 10h
#define CH1_GCAL_MSB 11h
#define CH1_GCAL_LSB 12h
#define CH2_CFG 13h
#define CH2_OCAL_MSB 14h
#define CH2_OCAL_LSB 15h
#define CH2_GCAL_MSB 16h
#define CH2_GCAL_LSB 17h
#define CH3_CFG 18h
#define CH3_OCAL_MSB 19h
#define CH3_OCAL_LSB 1Ah
#define CH3_GCAL_MSB 1Bh
#define CH3_GCAL_LSB 1Ch
#define REGMAP_CRC 3Eh
#define RESERVED 3Fh
