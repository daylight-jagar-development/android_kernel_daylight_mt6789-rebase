#ifndef _NT36523N_UXGA_SHARP_H_
#define _NT36523N_UXGA_SHARP_H_

/* --------------------------------------------------------------------------- */
/* Local Functions */
/* --------------------------------------------------------------------------- */
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

#ifndef EIO
#define EIO 5 /* I/O error */
#endif

#ifndef EINVAL
#define EINVAL 22 /* Invalid argument */
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef BIT
#define BIT(bit) (1UL << (bit))
#endif

#ifdef CUSTOM_DISPLAY_AREA
/* ------------------------------------- */
/* Display area */
/* ------------------------------------- */
struct SHARP_DISP_AREA {
    int left, top, right, bottom;
};

/* The Width/Height of the display area must be even */
static struct SHARP_DISP_AREA display_rect_ts = {0 +  0, 0 +  0, 1200 -  0, 1600 -  0};
#define display_rect_pre_ts display_rect_ts
static struct SHARP_DISP_AREA display_rect_es = {0 +  8, 0 +  8, 1200 -  8, 1600 -  8};
#define display_rect_pre_es display_rect_es
#define display_rect_es2_1 display_rect_es
#define display_rect_es2_2 display_rect_es
#define display_rect_es3 display_rect_es
#define display_rect_es2_final display_rect_es
static struct SHARP_DISP_AREA display_rect_cs = {0 +  8, 0 +  8, 1200 -  8, 1600 -  8};
static struct SHARP_DISP_AREA display_rect_mp = {0 +  8, 0 +  8, 1200 -  8, 1600 -  8};
#define display_rect_latest display_rect_mp
#define display_rect_unknown display_rect_ts
#define display_rect_not_found display_rect_mp
#endif

/* ------------------------------------- */
/* DSI DCS CMD */
/* ------------------------------------- */
#ifndef MIPI_DCS_ENTER_SLEEP_MODE
#define MIPI_DCS_ENTER_SLEEP_MODE   (0x10)
#endif

#ifndef MIPI_DCS_EXIT_SLEEP_MODE
#define MIPI_DCS_EXIT_SLEEP_MODE    (0x11)
#endif

#ifndef MIPI_DCS_SET_DISPLAY_OFF
#define MIPI_DCS_SET_DISPLAY_OFF    (0x28)
#endif

#ifndef MIPI_DCS_SET_DISPLAY_ON
#define MIPI_DCS_SET_DISPLAY_ON     (0x29)
#endif

#define REGFLAG_MDELAY              (0xFFFC)
#define REGFLAG_UDELAY              (0xFFFB)
#define REGFLAG_END_OF_TABLE        (0xFFFD)
#define REGFLAG_RESET_LOW           (0xFFFE)
#define REGFLAG_RESET_HIGH          (0xFFFF)

struct LCM_setting_table {
    unsigned int cmd;
    unsigned int count;
    unsigned char para_list[64];
};

#if defined(LK)
#define DCS_TABLE(_cmd, _count, _para...) {_cmd, _count, {_para} }
#else /* Kernel */
#define DCS_TABLE(_cmd, _count, _para...) {_cmd, (_count + 1), {(unsigned char)_cmd, _para} }
#endif

static struct LCM_setting_table page_switch_cmd1[] = { DCS_TABLE(0xFF, 1, 0x10), };

static struct LCM_setting_table page_switch_cmd2_p0[] = { DCS_TABLE(0xFF, 1, 0x20), };

static struct LCM_setting_table page_switch_cmd2_p1[] = { DCS_TABLE(0xFF, 1, 0x21), };

static struct LCM_setting_table page_switch_cmd2_p2[] = { DCS_TABLE(0xFF, 1, 0x22), };

static struct LCM_setting_table page_switch_cmd2_p3[] = { DCS_TABLE(0xFF, 1, 0x23), };

static struct LCM_setting_table page_switch_cmd2_p4[] = { DCS_TABLE(0xFF, 1, 0x24), };

static struct LCM_setting_table page_switch_cmd2_p5[] = { DCS_TABLE(0xFF, 1, 0x25), };

static struct LCM_setting_table page_switch_cmd2_p6[] = { DCS_TABLE(0xFF, 1, 0x26), };

static struct LCM_setting_table page_switch_cmd2_p7[] = { DCS_TABLE(0xFF, 1, 0x27), };

#if defined(LCM_SET_DISPLAY_ON_DELAY) || defined(CONFIG_DRM_MTK_ICOM_LCM_SET_DISPLAY_ON_DELAY)
/* to reduce init time, we move 120ms delay to lcm_set_display_on()/sharp_display_on() !! */
static struct LCM_setting_table set_display_on[] = {
    // DCS_TABLE(REGFLAG_MDELAY, 2000), /* only for the test of the BLACK output issue */
    DCS_TABLE(MIPI_DCS_SET_DISPLAY_ON, 0),
};
#endif

static struct LCM_setting_table set_sleep_out_display_on[] = {
    // DCS_TABLE(0xFF, 1, 0x10),
    /* Sleep out */
    DCS_TABLE(MIPI_DCS_EXIT_SLEEP_MODE, 0),
    DCS_TABLE(REGFLAG_MDELAY, 121), /* min. 120ms */
    /* Display On*/
    DCS_TABLE(MIPI_DCS_SET_DISPLAY_ON, 0),
};

static struct LCM_setting_table set_display_off[] = {
    // DCS_TABLE(0xFF, 1, 0x10),
    DCS_TABLE(MIPI_DCS_SET_DISPLAY_OFF, 0),
    DCS_TABLE(MIPI_DCS_ENTER_SLEEP_MODE, 0),
    DCS_TABLE(REGFLAG_MDELAY, 62), /* min. 60ms */
};

/* ------------------------------------- */
/* Initialization code: 20230912_Iinitial sequence(Tentative)_rev.K.xlsx */
/* ------------------------------------- */
static struct LCM_setting_table init_es3_120hz_3x_dsc[] = {
    DCS_TABLE(0xFF, 1, 0x20),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x18, 1, 0x04),
    DCS_TABLE(0x19, 1, 0x02),

    /*
     * The BLACK output issue between "Sleep Out" and "Display On"
     * is solved in OTP/MTP settings
     */

    /* The "VFP low frame rate flicker" issue is solved in OTP/MTP settings */

    /* The "Display Shearing" issue is solved in OTP/MTP settings */

    DCS_TABLE(0xFF, 1, 0x10),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x35, 1, 0x00),

    /* Sleep out */
    DCS_TABLE(MIPI_DCS_EXIT_SLEEP_MODE, 0),
#if !(defined(LCM_SET_DISPLAY_ON_DELAY) || defined(CONFIG_DRM_MTK_ICOM_LCM_SET_DISPLAY_ON_DELAY))
    DCS_TABLE(REGFLAG_MDELAY, 121), /* min. 120ms */
    /* Display On*/
    DCS_TABLE(MIPI_DCS_SET_DISPLAY_ON, 0),
#endif
};

/* ------------------------------------- */
/* Initialization code: 20230912_Iinitial sequence(Tentative)_rev.J.xlsx */
/* ------------------------------------- */
static struct LCM_setting_table init_es2_2_120hz_3x_dsc[] = {
    DCS_TABLE(0xFF, 1, 0x20),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x18, 1, 0x04),
    DCS_TABLE(0x19, 1, 0x02),

    /*
     * The BLACK output issue between "Sleep Out" and "Display On"
     * is solved in OTP/MTP settings
     */

    /* Enable low frame rate 120Hz-1Hz: Solved the VFP flicker issue */
    DCS_TABLE(0xFF, 1, 0x24),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0xC2, 3, 0xC2, 0x50, 0x50),

    /* The "Display Shearing" issue is solved in OTP/MTP settings */

    DCS_TABLE(0xFF, 1, 0x10),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x35, 1, 0x00),

    /* Sleep out */
    DCS_TABLE(MIPI_DCS_EXIT_SLEEP_MODE, 0),
#if !(defined(LCM_SET_DISPLAY_ON_DELAY) || defined(CONFIG_DRM_MTK_ICOM_LCM_SET_DISPLAY_ON_DELAY))
    DCS_TABLE(REGFLAG_MDELAY, 121), /* min. 120ms */
    /* Display On*/
    DCS_TABLE(MIPI_DCS_SET_DISPLAY_ON, 0),
#endif
};

/* ------------------------------------- */
/* Initialization code: 20230525_Iinitial sequence(Tentative)_rev.H.xlsx */
/* ------------------------------------- */
static struct LCM_setting_table init_es_120hz_3x_dsc[] = {
    DCS_TABLE(0xFF, 1, 0x20),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x18, 1, 0x04),
    DCS_TABLE(0x19, 1, 0x02),

    /* Solved the BLACK output issue between "Sleep Out" and "Display On" */
    DCS_TABLE(0xFF, 1, 0x24),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x63, 1, 0x60),

    /* Enable low frame rate 120Hz-1Hz: Solved the VFP flicker issue */
    //DCS_TABLE(0xFF, 1, 0x24),
    //DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0xC2, 3, 0xC2, 0x50, 0x50),

    /* The "Display Shearing" issue is solved in OTP/MTP settings */

    DCS_TABLE(0xFF, 1, 0x10),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x35, 1, 0x00),

    /* Sleep out */
    DCS_TABLE(MIPI_DCS_EXIT_SLEEP_MODE, 0),
#if !(defined(LCM_SET_DISPLAY_ON_DELAY) || defined(CONFIG_DRM_MTK_ICOM_LCM_SET_DISPLAY_ON_DELAY))
    DCS_TABLE(REGFLAG_MDELAY, 121), /* min. 120ms */
    /* Display On*/
    DCS_TABLE(MIPI_DCS_SET_DISPLAY_ON, 0),
#endif
};

/* ------------------------------------- */
/* Initialization code: 20230421_Iinitial sequence(Tentative)_rev.G.xlsx */
/* ------------------------------------- */
static struct LCM_setting_table init_ts_120hz_3x_dsc[] = {
    DCS_TABLE(0xFF, 1, 0x20),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x05, 1, 0xB9),
    DCS_TABLE(0x07, 1, 0x67),
    DCS_TABLE(0x08, 1, 0x53),
    DCS_TABLE(0x6D, 1, 0x00),
    DCS_TABLE(0x18, 1, 0x04),
    DCS_TABLE(0x19, 1, 0x02),

    DCS_TABLE(0xFF, 1, 0x24),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x5B, 1, 0x70),
    DCS_TABLE(0x01, 1, 0x01),
    DCS_TABLE(0x17, 1, 0x21),

    /* Solved the BLACK output issue between "Sleep Out" and "Display On" */
    //DCS_TABLE(0xFF, 1, 0x24),
    //DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x63, 1, 0x60),

    /* Solved the "Display Shearing" issue */
    DCS_TABLE(0xFF, 1, 0x25),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x0D, 1, 0x18), /* Solution 2 NO.B: DSC SLICE Height=20; DSC_Slice_delay[5:0]=0x18; VFP=54 */

    DCS_TABLE(0xFF, 1, 0x10),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x35, 1, 0x00),

    /* Sleep out */
    DCS_TABLE(MIPI_DCS_EXIT_SLEEP_MODE, 0),
#if !(defined(LCM_SET_DISPLAY_ON_DELAY) || defined(CONFIG_DRM_MTK_ICOM_LCM_SET_DISPLAY_ON_DELAY))
    DCS_TABLE(REGFLAG_MDELAY, 121), /* min. 120ms */
    /* Display On*/
    DCS_TABLE(MIPI_DCS_SET_DISPLAY_ON, 0),
#endif
};

static struct LCM_setting_table init_ts_120hz_3x_dsc_vfp[] = {
    DCS_TABLE(0xFF, 1, 0x20),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x05, 1, 0xB9),
    DCS_TABLE(0x07, 1, 0x67),
    DCS_TABLE(0x08, 1, 0x53),
    DCS_TABLE(0x6D, 1, 0x00),
    DCS_TABLE(0x18, 1, 0x04),
    DCS_TABLE(0x19, 1, 0x02),

    DCS_TABLE(0xFF, 1, 0x24),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x5B, 1, 0x70),
    DCS_TABLE(0x01, 1, 0x01),
    DCS_TABLE(0x17, 1, 0x21),

    /* Solved the BLACK output issue between "Sleep Out" and "Display On" */
    //DCS_TABLE(0xFF, 1, 0x24),
    //DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x63, 1, 0x60),

    /* Enable low frame rate 120Hz-1Hz: Solved the VFP flicker issue */
    //DCS_TABLE(0xFF, 1, 0x24),
    //DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0xC2, 3, 0xC2, 0x50, 0x50),

    /* Solved the "Display Shearing" issue */
    DCS_TABLE(0xFF, 1, 0x25),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x0D, 1, 0x18), /* Solution 2 NO.B: DSC SLICE Height=20; DSC_Slice_delay[5:0]=0x18; VFP=54 */

    DCS_TABLE(0xFF, 1, 0x10),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x35, 1, 0x00),

    /* Sleep out */
    DCS_TABLE(MIPI_DCS_EXIT_SLEEP_MODE, 0),
#if !(defined(LCM_SET_DISPLAY_ON_DELAY) || defined(CONFIG_DRM_MTK_ICOM_LCM_SET_DISPLAY_ON_DELAY))
    DCS_TABLE(REGFLAG_MDELAY, 121), /* min. 120ms */
    /* Display On*/
    DCS_TABLE(MIPI_DCS_SET_DISPLAY_ON, 0),
#endif
};

static struct LCM_setting_table init_ts_120hz_3x_dsc_frameskip_60hz[] = {
    DCS_TABLE(0xFF, 1, 0x20),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x05, 1, 0xB9),
    DCS_TABLE(0x07, 1, 0x67),
    DCS_TABLE(0x08, 1, 0x53),
    DCS_TABLE(0x6D, 1, 0x00),
    DCS_TABLE(0x18, 1, 0x04),
    DCS_TABLE(0x19, 1, 0x02),

    DCS_TABLE(0xFF, 1, 0x24),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x5B, 1, 0x70),
    DCS_TABLE(0x01, 1, 0x01),
    DCS_TABLE(0x17, 1, 0x21),

    /* Solved the BLACK output issue between "Sleep Out" and "Display On" */
    //DCS_TABLE(0xFF, 1, 0x24),
    //DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x63, 1, 0x60),

    /* Solved the "Display Shearing" issue */
    DCS_TABLE(0xFF, 1, 0x25),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x0D, 1, 0x18), /* Solution 2 NO.B: DSC SLICE Height=20; DSC_Slice_delay[5:0]=0x18; VFP=54 */

    DCS_TABLE(0xFF, 1, 0x10),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x35, 1, 0x00),

    /* Sleep out */
    DCS_TABLE(MIPI_DCS_EXIT_SLEEP_MODE, 0),
#if !(defined(LCM_SET_DISPLAY_ON_DELAY) || defined(CONFIG_DRM_MTK_ICOM_LCM_SET_DISPLAY_ON_DELAY))
    DCS_TABLE(REGFLAG_MDELAY, 121), /* min. 120ms */
    /* Display On*/
    DCS_TABLE(MIPI_DCS_SET_DISPLAY_ON, 0),
    DCS_TABLE(0xFF, 1, 0x25),
    DCS_TABLE(0xF2, 1, 0x01),
    DCS_TABLE(0xF3, 1, 0x01),
    DCS_TABLE(0xFF, 1, 0x10),
    DCS_TABLE(0xFB, 1, 0x01),
#endif
};

static struct LCM_setting_table set_frameskip_120hz[] = {
    DCS_TABLE(0xFF, 1, 0x25),
    DCS_TABLE(0xF2, 1, 0x00),
    DCS_TABLE(0xF3, 1, 0x00),
    DCS_TABLE(0xFF, 1, 0x10),
    DCS_TABLE(0xFB, 1, 0x01),
};

static struct LCM_setting_table set_frameskip_60hz[] = {
    DCS_TABLE(0xFF, 1, 0x25),
    DCS_TABLE(0xF2, 1, 0x01),
    DCS_TABLE(0xF3, 1, 0x01),
    DCS_TABLE(0xFF, 1, 0x10),
    DCS_TABLE(0xFB, 1, 0x01),
};

static struct LCM_setting_table set_frameskip_30hz[] = {
    DCS_TABLE(0xFF, 1, 0x25),
    DCS_TABLE(0xF2, 1, 0x03),
    DCS_TABLE(0xF3, 1, 0x01),
    DCS_TABLE(0xFF, 1, 0x10),
    DCS_TABLE(0xFB, 1, 0x01),
};

static struct LCM_setting_table set_frameskip_10hz[] = {
    DCS_TABLE(0xFF, 1, 0x25),
    DCS_TABLE(0xF2, 1, 0x01),
    DCS_TABLE(0xF3, 1, 0x0B),
    DCS_TABLE(0xFF, 1, 0x10),
    DCS_TABLE(0xFB, 1, 0x01),
};

static struct LCM_setting_table set_frameskip_1hz[] = {
    DCS_TABLE(0xFF, 1, 0x25),
    DCS_TABLE(0xF2, 1, 0x75),
    DCS_TABLE(0xF3, 1, 0x03),
    DCS_TABLE(0xFF, 1, 0x10),
    DCS_TABLE(0xFB, 1, 0x01),
};

static struct LCM_setting_table set_vfp_frameskip_1hz[] = {
    DCS_TABLE(0xFF, 1, 0x25),
    DCS_TABLE(0xF2, 1, 0x09),
    DCS_TABLE(0xF3, 1, 0x01),
    DCS_TABLE(0xFF, 1, 0x10),
    DCS_TABLE(0xFB, 1, 0x01),
};

static struct LCM_setting_table init_pre_ts_60hz_no_dsc[] = {
    DCS_TABLE(0xFF, 1, 0x20),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x00, 1, 0x00),
    DCS_TABLE(0x05, 1, 0xB1),
    DCS_TABLE(0x06, 1, 0xC0),
    DCS_TABLE(0x07, 1, 0x69),
    DCS_TABLE(0x08, 1, 0x55),
    DCS_TABLE(0x0E, 1, 0x87),
    DCS_TABLE(0x0F, 1, 0x73),
    DCS_TABLE(0x58, 1, 0x60),
    DCS_TABLE(0x6E, 1, 0x00),
    DCS_TABLE(0x88, 1, 0x00),
    DCS_TABLE(0x89, 1, 0x51),
    DCS_TABLE(0x8A, 1, 0x51),
    DCS_TABLE(0x94, 1, 0x00),
    DCS_TABLE(0x95, 1, 0xD7),
    DCS_TABLE(0x96, 1, 0xD7),
    DCS_TABLE(0x18, 1, 0x04),
    DCS_TABLE(0x19, 1, 0x02),
    DCS_TABLE(0x62, 1, 0x36),
    DCS_TABLE(0x63, 1, 0xA6),
    DCS_TABLE(0x09, 1, 0xAB),
    DCS_TABLE(0x0A, 1, 0x88),
    DCS_TABLE(0x0B, 1, 0x17),
    DCS_TABLE(0x0C, 1, 0x07),
    DCS_TABLE(0x33, 1, 0x35),
    DCS_TABLE(0x10, 1, 0x0E),
    DCS_TABLE(0x60, 1, 0x99),
    DCS_TABLE(0x58, 1, 0x40),
    DCS_TABLE(0xFF, 1, 0x24),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x60, 1, 0x96),
    DCS_TABLE(0x61, 1, 0x40),
#if 0
    DCS_TABLE(0x63, 1, 0x64),
#else
    /* Solved the BLACK output issue between "Sleep Out" and "Display On" */
    DCS_TABLE(0x63, 1, 0x60),
#endif
    DCS_TABLE(0x91, 1, 0x20),
    DCS_TABLE(0x92, 1, 0xC6),
    DCS_TABLE(0x93, 1, 0x1C),
    DCS_TABLE(0x94, 1, 0x08),
    DCS_TABLE(0x00, 1, 0x08),
    DCS_TABLE(0x01, 1, 0x26),
    DCS_TABLE(0x02, 1, 0x03),
    DCS_TABLE(0x03, 1, 0x03),
    DCS_TABLE(0x04, 1, 0x03),
    DCS_TABLE(0x05, 1, 0x03),
    DCS_TABLE(0x06, 1, 0x05),
    DCS_TABLE(0x07, 1, 0x04),
    DCS_TABLE(0x08, 1, 0x0D),
    DCS_TABLE(0x09, 1, 0x0F),
    DCS_TABLE(0x0A, 1, 0x0C),
    DCS_TABLE(0x0B, 1, 0x0E),
    DCS_TABLE(0x0C, 1, 0x27),
    DCS_TABLE(0x0D, 1, 0x27),
    DCS_TABLE(0x0E, 1, 0x27),
    DCS_TABLE(0x0F, 1, 0x27),
    DCS_TABLE(0x10, 1, 0x03),
    DCS_TABLE(0x11, 1, 0x03),
    DCS_TABLE(0x12, 1, 0x03),
    DCS_TABLE(0x13, 1, 0x03),
    DCS_TABLE(0x14, 1, 0x03),
    DCS_TABLE(0x15, 1, 0x03),
    DCS_TABLE(0x16, 1, 0x08),
    DCS_TABLE(0x17, 1, 0x26),
    DCS_TABLE(0x18, 1, 0x03),
    DCS_TABLE(0x19, 1, 0x03),
    DCS_TABLE(0x1A, 1, 0x03),
    DCS_TABLE(0x1B, 1, 0x03),
    DCS_TABLE(0x1C, 1, 0x05),
    DCS_TABLE(0x1D, 1, 0x04),
    DCS_TABLE(0x1E, 1, 0x0D),
    DCS_TABLE(0x1F, 1, 0x0F),
    DCS_TABLE(0x20, 1, 0x0C),
    DCS_TABLE(0x21, 1, 0x0E),
    DCS_TABLE(0x22, 1, 0x27),
    DCS_TABLE(0x23, 1, 0x27),
    DCS_TABLE(0x24, 1, 0x27),
    DCS_TABLE(0x25, 1, 0x27),
    DCS_TABLE(0x26, 1, 0x03),
    DCS_TABLE(0x27, 1, 0x03),
    DCS_TABLE(0x28, 1, 0x03),
    DCS_TABLE(0x29, 1, 0x03),
    DCS_TABLE(0x2A, 1, 0x03),
    DCS_TABLE(0x2B, 1, 0x03),
    DCS_TABLE(0x98, 1, 0x80),
    DCS_TABLE(0x2F, 1, 0x02),
    DCS_TABLE(0x30, 1, 0x01),
    DCS_TABLE(0x33, 1, 0x01),
    DCS_TABLE(0x34, 1, 0x02),
    DCS_TABLE(0x37, 1, 0x22),
    DCS_TABLE(0x39, 1, 0x01),
    DCS_TABLE(0x3A, 1, 0x02),
    DCS_TABLE(0x3B, 1, 0x60),
    DCS_TABLE(0x3D, 1, 0x02),
    DCS_TABLE(0xAB, 1, 0x22),
    DCS_TABLE(0x3F, 1, 0x05),
    DCS_TABLE(0x43, 1, 0x05),
    DCS_TABLE(0x47, 1, 0x22),
    DCS_TABLE(0x49, 1, 0x01),
    DCS_TABLE(0x4A, 1, 0x02),
    DCS_TABLE(0x4B, 1, 0x60),
    DCS_TABLE(0x4C, 1, 0x01),
    DCS_TABLE(0xF7, 1, 0x22),
    DCS_TABLE(0x4D, 1, 0x21),
    DCS_TABLE(0x4E, 1, 0x43),
    DCS_TABLE(0x4F, 1, 0x65),
    DCS_TABLE(0x50, 1, 0x87),
    DCS_TABLE(0x51, 1, 0x78),
    DCS_TABLE(0x52, 1, 0x56),
    DCS_TABLE(0x53, 1, 0x34),
    DCS_TABLE(0x54, 1, 0x12),
    DCS_TABLE(0x55, 2, 0x03, 0x03),
    DCS_TABLE(0x56, 1, 0x04),
    DCS_TABLE(0x58, 1, 0x10),
    DCS_TABLE(0x59, 1, 0x21),
    DCS_TABLE(0x5A, 1, 0x02),
    DCS_TABLE(0x5B, 1, 0x40),
    DCS_TABLE(0x5C, 1, 0x00),
    DCS_TABLE(0x5D, 1, 0x00),
    DCS_TABLE(0x5E, 2, 0x00, 0x03),
    DCS_TABLE(0x5F, 1, 0x00),
    DCS_TABLE(0x96, 1, 0x00),
    DCS_TABLE(0xA5, 1, 0xAA),
    DCS_TABLE(0xB6, 12, 0x05, 0x00, 0x05, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x05, 0x05,
                        0x00, 0x00),
    DCS_TABLE(0xBB, 1, 0x80),
    DCS_TABLE(0xBC, 5, 0x00, 0x00, 0x03, 0x00, 0x00),
    DCS_TABLE(0xDB, 1, 0xF2),
    DCS_TABLE(0xDC, 1, 0x6E),
    DCS_TABLE(0xFF, 1, 0x25),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x05, 1, 0x04),
    /* Solved the "Display Shearing" issue */
    DCS_TABLE(0x0D, 1, 0x18), /* Solution 2 NO.B: DSC SLICE Height=20; DSC_Slice_delay[5:0]=0x18; VFP=54 */
    DCS_TABLE(0x19, 1, 0x07),
    DCS_TABLE(0x1B, 1, 0x11),
    DCS_TABLE(0xC2, 1, 0xC0),
    DCS_TABLE(0xC5, 1, 0x17),
    DCS_TABLE(0xC8, 1, 0x07),
    DCS_TABLE(0xC0, 1, 0x0C),
    DCS_TABLE(0xF4, 1, 0xD8),
    DCS_TABLE(0x13, 1, 0x04),
    DCS_TABLE(0x14, 1, 0x9A),
    DCS_TABLE(0xDB, 1, 0x03),
    DCS_TABLE(0xDC, 1, 0x9F),
    DCS_TABLE(0xFF, 1, 0x26),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x00, 1, 0xA0),
    DCS_TABLE(0x04, 1, 0x50),
    DCS_TABLE(0xFF, 1, 0x27),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0xC0, 1, 0x10),
    DCS_TABLE(0xFF, 1, 0x2A),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x64, 1, 0x96),
    DCS_TABLE(0x67, 1, 0x96),
    DCS_TABLE(0x68, 1, 0x63),
    DCS_TABLE(0x6A, 1, 0x96),
    DCS_TABLE(0x7F, 1, 0x96),
    DCS_TABLE(0x80, 1, 0x63),
    DCS_TABLE(0x82, 1, 0x96),
    DCS_TABLE(0xA2, 1, 0xFF),
    DCS_TABLE(0xA3, 1, 0xFF),
    DCS_TABLE(0xA4, 1, 0xFF),
    DCS_TABLE(0xA5, 1, 0xFF),
    DCS_TABLE(0xA6, 1, 0xFF),
    DCS_TABLE(0xF1, 1, 0x03),
    DCS_TABLE(0x97, 1, 0x3C),
    DCS_TABLE(0x98, 1, 0x02),
    DCS_TABLE(0x99, 1, 0x95),
    DCS_TABLE(0x9A, 1, 0x06),
    DCS_TABLE(0x9B, 1, 0x00),
    DCS_TABLE(0x9C, 1, 0x0B),
    DCS_TABLE(0x9D, 1, 0x0A),
    DCS_TABLE(0x9E, 1, 0x90),
    DCS_TABLE(0xF2, 1, 0x3C),
    DCS_TABLE(0xF3, 1, 0x02),
    DCS_TABLE(0xF4, 1, 0x95),
    DCS_TABLE(0xF5, 1, 0x06),
    DCS_TABLE(0xF6, 1, 0x00),
    DCS_TABLE(0xF7, 1, 0x0B),
    DCS_TABLE(0xF8, 1, 0x0A),
    DCS_TABLE(0xF9, 1, 0x90),
    DCS_TABLE(0xFF, 1, 0x10),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x35, 1, 0x00),
    DCS_TABLE(0x90, 1, 0x00),
    DCS_TABLE(0x91, 16, 0x89, 0xA8, 0x00, 0x0C, 0xD2,
                        0x00, 0x02, 0x25, 0x01, 0x14,
                        0x00, 0x07, 0x09, 0x75, 0x08,
                        0x7A),
    DCS_TABLE(0x92, 2, 0x10, 0xF0),
    DCS_TABLE(0xBB, 1, 0x13),
    /* Sleep out */
    DCS_TABLE(MIPI_DCS_EXIT_SLEEP_MODE, 0),
#if !(defined(LCM_SET_DISPLAY_ON_DELAY) || defined(CONFIG_DRM_MTK_ICOM_LCM_SET_DISPLAY_ON_DELAY))
    DCS_TABLE(REGFLAG_MDELAY, 121), /* min. 120ms */
    /* Display On*/
    DCS_TABLE(MIPI_DCS_SET_DISPLAY_ON, 0),
#endif
};
static struct LCM_setting_table init_pre_ts_120hz_3x_dsc[] = {
    DCS_TABLE(0xFF, 1, 0x20),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x00, 1, 0x00),
    DCS_TABLE(0x05, 1, 0xB1),
    DCS_TABLE(0x06, 1, 0xC0),
    DCS_TABLE(0x07, 1, 0x69),
    DCS_TABLE(0x08, 1, 0x55),
    DCS_TABLE(0x0E, 1, 0x87),
    DCS_TABLE(0x0F, 1, 0x73),
    DCS_TABLE(0x58, 1, 0x60),
    DCS_TABLE(0x6E, 1, 0x00),
    DCS_TABLE(0x88, 1, 0x00),
    DCS_TABLE(0x89, 1, 0x83),
    DCS_TABLE(0x8A, 1, 0x83),
    DCS_TABLE(0x94, 1, 0x00),
    DCS_TABLE(0x95, 1, 0xD7),
    DCS_TABLE(0x96, 1, 0xD7),
    DCS_TABLE(0x18, 1, 0x04),
    DCS_TABLE(0x19, 1, 0x02),
    DCS_TABLE(0x62, 1, 0x36),
    DCS_TABLE(0x63, 1, 0xA6),
    DCS_TABLE(0x09, 1, 0xAB),
    DCS_TABLE(0x0A, 1, 0x88),
    DCS_TABLE(0x0B, 1, 0x17),
    DCS_TABLE(0x0C, 1, 0x07),
    DCS_TABLE(0x33, 1, 0x35),
    DCS_TABLE(0x10, 1, 0x0E),
    DCS_TABLE(0x60, 1, 0x99),
    DCS_TABLE(0x58, 1, 0x40),
    DCS_TABLE(0xFF, 1, 0x24),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x60, 1, 0x96),
    DCS_TABLE(0x61, 1, 0x40),
#if 0
    DCS_TABLE(0x63, 1, 0x64),
#else
    /* Solved the BLACK output issue between "Sleep Out" and "Display On" */
    DCS_TABLE(0x63, 1, 0x60),
#endif
    DCS_TABLE(0x91, 1, 0x00),
    DCS_TABLE(0x92, 1, 0xB7),
    DCS_TABLE(0x93, 1, 0x1C),
    DCS_TABLE(0x94, 1, 0x08),
    DCS_TABLE(0x00, 1, 0x08),
    DCS_TABLE(0x01, 1, 0x26),
    DCS_TABLE(0x02, 1, 0x03),
    DCS_TABLE(0x03, 1, 0x03),
    DCS_TABLE(0x04, 1, 0x03),
    DCS_TABLE(0x05, 1, 0x03),
    DCS_TABLE(0x06, 1, 0x05),
    DCS_TABLE(0x07, 1, 0x04),
    DCS_TABLE(0x08, 1, 0x0D),
    DCS_TABLE(0x09, 1, 0x0F),
    DCS_TABLE(0x0A, 1, 0x0C),
    DCS_TABLE(0x0B, 1, 0x0E),
    DCS_TABLE(0x0C, 1, 0x27),
    DCS_TABLE(0x0D, 1, 0x27),
    DCS_TABLE(0x0E, 1, 0x27),
    DCS_TABLE(0x0F, 1, 0x27),
    DCS_TABLE(0x10, 1, 0x03),
    DCS_TABLE(0x11, 1, 0x03),
    DCS_TABLE(0x12, 1, 0x03),
    DCS_TABLE(0x13, 1, 0x03),
    DCS_TABLE(0x14, 1, 0x03),
    DCS_TABLE(0x15, 1, 0x03),
    DCS_TABLE(0x16, 1, 0x08),
    DCS_TABLE(0x17, 1, 0x26),
    DCS_TABLE(0x18, 1, 0x03),
    DCS_TABLE(0x19, 1, 0x03),
    DCS_TABLE(0x1A, 1, 0x03),
    DCS_TABLE(0x1B, 1, 0x03),
    DCS_TABLE(0x1C, 1, 0x05),
    DCS_TABLE(0x1D, 1, 0x04),
    DCS_TABLE(0x1E, 1, 0x0D),
    DCS_TABLE(0x1F, 1, 0x0F),
    DCS_TABLE(0x20, 1, 0x0C),
    DCS_TABLE(0x21, 1, 0x0E),
    DCS_TABLE(0x22, 1, 0x27),
    DCS_TABLE(0x23, 1, 0x27),
    DCS_TABLE(0x24, 1, 0x27),
    DCS_TABLE(0x25, 1, 0x27),
    DCS_TABLE(0x26, 1, 0x03),
    DCS_TABLE(0x27, 1, 0x03),
    DCS_TABLE(0x28, 1, 0x03),
    DCS_TABLE(0x29, 1, 0x03),
    DCS_TABLE(0x2A, 1, 0x03),
    DCS_TABLE(0x2B, 1, 0x03),
    DCS_TABLE(0x98, 1, 0x80),
    DCS_TABLE(0x2F, 1, 0x02),
    DCS_TABLE(0x30, 1, 0x01),
    DCS_TABLE(0x33, 1, 0x01),
    DCS_TABLE(0x34, 1, 0x02),
    DCS_TABLE(0x37, 1, 0x22),
    DCS_TABLE(0x39, 1, 0x00),
    DCS_TABLE(0x3A, 1, 0x02),
    DCS_TABLE(0x3B, 1, 0xB0),
    DCS_TABLE(0x3D, 1, 0x02),
    DCS_TABLE(0xAB, 1, 0x22),
    DCS_TABLE(0x3F, 1, 0x05),
    DCS_TABLE(0x43, 1, 0x05),
    DCS_TABLE(0x47, 1, 0x22),
    DCS_TABLE(0x49, 1, 0x00),
    DCS_TABLE(0x4A, 1, 0x02),
    DCS_TABLE(0x4B, 1, 0xB0),
    DCS_TABLE(0x4C, 1, 0x01),
    DCS_TABLE(0xF7, 1, 0x22),
    DCS_TABLE(0x4D, 1, 0x21),
    DCS_TABLE(0x4E, 1, 0x43),
    DCS_TABLE(0x4F, 1, 0x65),
    DCS_TABLE(0x50, 1, 0x87),
    DCS_TABLE(0x51, 1, 0x78),
    DCS_TABLE(0x52, 1, 0x56),
    DCS_TABLE(0x53, 1, 0x34),
    DCS_TABLE(0x54, 1, 0x12),
    DCS_TABLE(0x55, 2, 0x03, 0x03),
    DCS_TABLE(0x56, 1, 0x04),
    DCS_TABLE(0x58, 1, 0x10),
    DCS_TABLE(0x59, 1, 0x20),
    DCS_TABLE(0x5A, 1, 0x02),
    DCS_TABLE(0x5B, 1, 0xA0),
    DCS_TABLE(0x5C, 1, 0x00),
    DCS_TABLE(0x5D, 1, 0x00),
    DCS_TABLE(0x5E, 2, 0x00, 0x03),
    DCS_TABLE(0x5F, 1, 0x00),
    DCS_TABLE(0x96, 1, 0x00),
    DCS_TABLE(0xA5, 1, 0xAA),
    DCS_TABLE(0xB6, 12, 0x05, 0x00, 0x05, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x05, 0x05,
                        0x00, 0x00),
    DCS_TABLE(0xBB, 1, 0x80),
    DCS_TABLE(0xBC, 5, 0x00, 0x00, 0x03, 0x00, 0x00),
    DCS_TABLE(0xC2, 3, 0xC2, 0x50, 0x50),
    DCS_TABLE(0xDB, 1, 0x72),
    DCS_TABLE(0xDC, 1, 0xB7),
    DCS_TABLE(0xFF, 1, 0x25),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x05, 1, 0x04),
    /* Solved the "Display Shearing" issue */
    DCS_TABLE(0x0D, 1, 0x18), /* Solution 2 NO.B: DSC SLICE Height=20; DSC_Slice_delay[5:0]=0x18; VFP=54 */
    DCS_TABLE(0x19, 1, 0x07),
    DCS_TABLE(0x1B, 1, 0x11),
    DCS_TABLE(0xC2, 1, 0xC0),
    DCS_TABLE(0xC5, 1, 0x17),
    DCS_TABLE(0xC8, 1, 0x07),
    DCS_TABLE(0xC0, 1, 0x0C),
    DCS_TABLE(0xF4, 1, 0xD8),
    DCS_TABLE(0x13, 1, 0x02),
    DCS_TABLE(0x14, 1, 0x4D),
    DCS_TABLE(0xDB, 1, 0x01),
    DCS_TABLE(0xDC, 1, 0xD0),
    DCS_TABLE(0xFF, 1, 0x26),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x00, 1, 0xA0),
    DCS_TABLE(0x04, 1, 0x50),
    DCS_TABLE(0xFF, 1, 0x27),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0xC0, 1, 0x10),
    DCS_TABLE(0xFF, 1, 0x2A),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x64, 1, 0x96),
    DCS_TABLE(0x67, 1, 0x96),
    DCS_TABLE(0x68, 1, 0x63),
    DCS_TABLE(0x6A, 1, 0x96),
    DCS_TABLE(0x7F, 1, 0x96),
    DCS_TABLE(0x80, 1, 0x63),
    DCS_TABLE(0x82, 1, 0x96),
    DCS_TABLE(0xA2, 1, 0xFF),
    DCS_TABLE(0xA3, 1, 0xFF),
    DCS_TABLE(0xA4, 1, 0xFF),
    DCS_TABLE(0xA5, 1, 0xFF),
    DCS_TABLE(0xA6, 1, 0xFF),
    DCS_TABLE(0xF1, 1, 0x03),
    DCS_TABLE(0x97, 1, 0x3C),
    DCS_TABLE(0x98, 1, 0x02),
    DCS_TABLE(0x99, 1, 0x95),
    DCS_TABLE(0x9A, 1, 0x06),
    DCS_TABLE(0x9B, 1, 0x00),
    DCS_TABLE(0x9C, 1, 0x0B),
    DCS_TABLE(0x9D, 1, 0x0A),
    DCS_TABLE(0x9E, 1, 0x90),
    DCS_TABLE(0xF2, 1, 0x3C),
    DCS_TABLE(0xF3, 1, 0x02),
    DCS_TABLE(0xF4, 1, 0x95),
    DCS_TABLE(0xF5, 1, 0x06),
    DCS_TABLE(0xF6, 1, 0x00),
    DCS_TABLE(0xF7, 1, 0x0B),
    DCS_TABLE(0xF8, 1, 0x0A),
    DCS_TABLE(0xF9, 1, 0x90),
    DCS_TABLE(0xFF, 1, 0x10),
    DCS_TABLE(0xFB, 1, 0x01),
    DCS_TABLE(0x35, 1, 0x00),
    DCS_TABLE(0x90, 1, 0x03),
    DCS_TABLE(0x91, 16, 0x89, 0xA8, 0x00, 0x14, 0xD2,
                        0x00, 0x02, 0x45, 0x01, 0xEC,
                        0x00, 0x08, 0x05, 0x7A, 0x04,
                        0x94),
    DCS_TABLE(0x92, 2, 0x10, 0xF0),
    DCS_TABLE(0xBB, 1, 0x13),
    /* Sleep out */
    DCS_TABLE(MIPI_DCS_EXIT_SLEEP_MODE, 0),
#if !(defined(LCM_SET_DISPLAY_ON_DELAY) || defined(CONFIG_DRM_MTK_ICOM_LCM_SET_DISPLAY_ON_DELAY))
    DCS_TABLE(REGFLAG_MDELAY, 121), /* min. 120ms */
    /* Display On*/
    DCS_TABLE(MIPI_DCS_SET_DISPLAY_ON, 0),
#endif
};

/* ------------------------------------- */
/* LCM timing */
/* ------------------------------------- */
// 60Hz / 1200RGB x 1600 / No Compression
#define HAC_60HZ_NO_DSC (1200)
#define HFP_60HZ_NO_DSC (30)
#define HSA_60HZ_NO_DSC (20)
#define HBP_60HZ_NO_DSC (60)
#define VAC_60HZ_NO_DSC (1600)
#define VFP_60HZ_NO_DSC (30)
#define VSA_60HZ_NO_DSC (2)
#define VBP_60HZ_NO_DSC (6)
#define DATA_RATE_60HZ_NO_DSC (850)
#define CLOCK_KHZ_60HZ_NO_DSC (128746)
// 120Hz / 1200RGB x 1600 / VESA 3X DSC
#define HAC_120HZ_3X_DSC (1200)
#define HFP_120HZ_3X_DSC (30)
#define HSA_120HZ_3X_DSC (20)
#define HBP_120HZ_3X_DSC (60)
#define VAC_120HZ_3X_DSC (1600)
#define VSA_120HZ_3X_DSC (2)
#define VBP_120HZ_3X_DSC (6)
/* Solved the "Display Shearing" issue: Solution 2 NO.B: DSC SLICE Height=20; DSC_Slice_delay[5:0]=0x18; VFP=54 */
#define VFP_120HZ_3X_DSC (54)
#define VFP_120HZ_3X_DSC_VFP_90HZ (608)
#define VFP_120HZ_3X_DSC_VFP_72HZ (1162)
#define VFP_120HZ_3X_DSC_VFP_60HZ (1716)
#define VFP_120HZ_3X_DSC_VFP_45HZ (2824)
#define VFP_120HZ_3X_DSC_VFP_30HZ (5040)
#define VFP_120HZ_3X_DSC_VFP_24HZ (6702)
#define VFP_120HZ_3X_DSC_VFP_15HZ (11688)
#define VFP_120HZ_3X_DSC_VFP_10HZ (18336)
#define VFP_120HZ_3X_DSC_VFP_6HZ  (31632)
#define DATA_RATE_120HZ_3X_DSC (672)
#define CLOCK_KHZ_120HZ_3X_DSC (261267)
#define CLOCK_KHZ_120HZ_3X_DSC_FRAMESKIP_60HZ (130634)
#define CLOCK_KHZ_120HZ_3X_DSC_FRAMESKIP_30HZ (65317)
#define CLOCK_KHZ_120HZ_3X_DSC_FRAMESKIP_10HZ (21773)
#define CLOCK_KHZ_120HZ_3X_DSC_FRAMESKIP_1HZ (2178)
#define CLOCK_KHZ_120HZ_3X_DSC_10HZ_VFP_FRAMESKIP_1HZ (26127)
#define CLOCK_KHZ_120HZ_3X_DSC_24HZ_VFP_FRAMESKIP_1HZ (10887)
#define CLOCK_KHZ_120HZ_3X_DSC_30HZ_VFP_FRAMESKIP_1HZ (8709)

/* DSC */
#define DSC_ENABLE                  (1)
#define DSC_SLICE_MODE              (1)
#define DSC_DSC_CFG                 (34)
#define DSC_VER                     (18)
#define DSC_RGB_SWAP                (0)
#define DSC_RCT_ON                  (1)
#define DSC_BIT_PER_CHANNEL         (8)
#define DSC_DSC_LINE_BUF_DEPTH      (9)
#define DSC_BP_ENABLE               (1)
#define DSC_BIT_PER_PIXEL           (128)
#define DSC_PIC_HEIGHT              (1600)
#define DSC_PIC_WIDTH               (1200)
#define DSC_SLICE_HEIGHT            (20)
#define DSC_SLICE_WIDTH             (600)
#define DSC_CHUNK_SIZE              (600)
#define DSC_XMIT_DELAY              (512)
#define DSC_DEC_DELAY               (581)
#define DSC_SCALE_VALUE             (32)
#define DSC_INCREMENT_INTERVAL      (492)
#define DSC_DECREMENT_INTERVAL      (8)
#define DSC_LINE_BPG_OFFSET         (13)
#define DSC_NFL_BPG_OFFSET          (1402)
#define DSC_SLICE_BPG_OFFSET        (1172)
#define DSC_INITIAL_OFFSET          (6144)
#define DSC_FINAL_OFFSET            (4336)
#define DSC_FLATNESS_MINQP          (3)
#define DSC_FLATNESS_MAXQP          (12)
#define DSC_RC_MODEL_SIZE           (8192)
#define DSC_RC_EDGE_FACTOR          (6)
#define DSC_RC_QUANT_INCR_LIMIT0    (11)
#define DSC_RC_QUANT_INCR_LIMIT1    (11)
#define DSC_RC_TGT_OFFSET_HI        (3)
#define DSC_RC_TGT_OFFSET_LO        (3)
#define DSC_RC_BUF_THRESH_0         (14)
#define DSC_RC_BUF_THRESH_1         (28)
#define DSC_RC_BUF_THRESH_2         (42)
#define DSC_RC_BUF_THRESH_3         (56)
#define DSC_RC_BUF_THRESH_4         (70)
#define DSC_RC_BUF_THRESH_5         (84)
#define DSC_RC_BUF_THRESH_6         (98)
#define DSC_RC_BUF_THRESH_7         (105)
#define DSC_RC_BUF_THRESH_8         (112)
#define DSC_RC_BUF_THRESH_9         (119)
#define DSC_RC_BUF_THRESH_10        (121)
#define DSC_RC_BUF_THRESH_11        (123)
#define DSC_RC_BUF_THRESH_12        (125)
#define DSC_RC_BUF_THRESH_13        (126)
#define DSC_RC_RANGE_0_RANGE_MIN_QP         (0)
#define DSC_RC_RANGE_0_RANGE_MAX_QP         (4)
#define DSC_RC_RANGE_0_RANGE_BPG_OFFSET     (2)
#define DSC_RC_RANGE_1_RANGE_MIN_QP         (0)
#define DSC_RC_RANGE_1_RANGE_MAX_QP         (4)
#define DSC_RC_RANGE_1_RANGE_BPG_OFFSET     (0)
#define DSC_RC_RANGE_2_RANGE_MIN_QP         (1)
#define DSC_RC_RANGE_2_RANGE_MAX_QP         (5)
#define DSC_RC_RANGE_2_RANGE_BPG_OFFSET     (0)
#define DSC_RC_RANGE_3_RANGE_MIN_QP         (1)
#define DSC_RC_RANGE_3_RANGE_MAX_QP         (6)
#define DSC_RC_RANGE_3_RANGE_BPG_OFFSET     (-2)
#define DSC_RC_RANGE_4_RANGE_MIN_QP         (3)
#define DSC_RC_RANGE_4_RANGE_MAX_QP         (7)
#define DSC_RC_RANGE_4_RANGE_BPG_OFFSET     (-4)
#define DSC_RC_RANGE_5_RANGE_MIN_QP         (3)
#define DSC_RC_RANGE_5_RANGE_MAX_QP         (7)
#define DSC_RC_RANGE_5_RANGE_BPG_OFFSET     (-6)
#define DSC_RC_RANGE_6_RANGE_MIN_QP         (3)
#define DSC_RC_RANGE_6_RANGE_MAX_QP         (7)
#define DSC_RC_RANGE_6_RANGE_BPG_OFFSET     (-8)
#define DSC_RC_RANGE_7_RANGE_MIN_QP         (3)
#define DSC_RC_RANGE_7_RANGE_MAX_QP         (8)
#define DSC_RC_RANGE_7_RANGE_BPG_OFFSET     (-8)
#define DSC_RC_RANGE_8_RANGE_MIN_QP         (3)
#define DSC_RC_RANGE_8_RANGE_MAX_QP         (9)
#define DSC_RC_RANGE_8_RANGE_BPG_OFFSET     (-8)
#define DSC_RC_RANGE_9_RANGE_MIN_QP         (3)
#define DSC_RC_RANGE_9_RANGE_MAX_QP         (10)
#define DSC_RC_RANGE_9_RANGE_BPG_OFFSET     (-10)
#define DSC_RC_RANGE_10_RANGE_MIN_QP        (5)
#define DSC_RC_RANGE_10_RANGE_MAX_QP        (10)
#define DSC_RC_RANGE_10_RANGE_BPG_OFFSET    (-10)
#define DSC_RC_RANGE_11_RANGE_MIN_QP        (5)
#define DSC_RC_RANGE_11_RANGE_MAX_QP        (11)
#define DSC_RC_RANGE_11_RANGE_BPG_OFFSET    (-12)
#define DSC_RC_RANGE_12_RANGE_MIN_QP        (5)
#define DSC_RC_RANGE_12_RANGE_MAX_QP        (11)
#define DSC_RC_RANGE_12_RANGE_BPG_OFFSET    (-12)
#define DSC_RC_RANGE_13_RANGE_MIN_QP        (9)
#define DSC_RC_RANGE_13_RANGE_MAX_QP        (12)
#define DSC_RC_RANGE_13_RANGE_BPG_OFFSET    (-12)
#define DSC_RC_RANGE_14_RANGE_MIN_QP        (12)
#define DSC_RC_RANGE_14_RANGE_MAX_QP        (13)
#define DSC_RC_RANGE_14_RANGE_BPG_OFFSET    (-12)

/* ------------------------------------- */
/* LCM */
/* ------------------------------------- */
// nt36523n special register,is not compare id register
#define NT36523N_ID_ADDR        (0x3B)
#define LCM_ID_NT36523N_SHARP	(0x3F)

#define PHYSICAL_WIDTH_UM       (160080)
#define PHYSICAL_HEIGHT_UM      (213440)

#define FRAME_WIDTH             (1200)
#define FRAME_HEIGHT            (1600)

#define SHARP_SAMPLE_ID1_ADDR   (0x27)
#define SHARP_SAMPLE_ID2_ADDR   (0x43)
#define SHARP_SAMPLE_ID3_ADDR   (0x28)
/* pre-TS */
#define SHARP_PRE_TS_ID1        (0x00)
#define SHARP_PRE_TS_ID2        (0x00)
#define SHARP_PRE_TS_ID3        (0x00)
/* TS */
#define SHARP_TS_ID1            (0x00)
#define SHARP_TS_ID2
#define SHARP_TS_ID3            (0x00)
/* pre-ES */
#define SHARP_PRE_ES_ID1        (0x10)
#define SHARP_PRE_ES_ID2
#define SHARP_PRE_ES_ID3        (0x00)
/* ES */
#define SHARP_ES_ID1            (0x10)
#define SHARP_ES_ID2
#define SHARP_ES_ID3
/* ES2-1 */
#define SHARP_ES2_1_ID1         (0x11)
#define SHARP_ES2_1_ID2
#define SHARP_ES2_1_ID3
/* ES2-2 */
#define SHARP_ES2_2_ID1         (0x12)
#define SHARP_ES2_2_ID2
#define SHARP_ES2_2_ID3
/* ES3 */
#define SHARP_ES3_ID1           (0x13)
#define SHARP_ES3_ID2
#define SHARP_ES3_ID3
/* ES2 Final */
#define SHARP_ES2_FINAL_ID1     (0x14)
#define SHARP_ES2_FINAL_ID2
#define SHARP_ES2_FINAL_ID3
/* CS */
#define SHARP_CS_ID1            (0x20)
#define SHARP_CS_ID2
#define SHARP_CS_ID3
/* MP */
#define SHARP_MP_ID1            (0x30)
#define SHARP_MP_ID2
#define SHARP_MP_ID3
/* Latest */
#define SHARP_LATEST_ID1        (SHARP_MP_ID1)
#define SHARP_LATEST_ID2        (SHARP_MP_ID2)
#define SHARP_LATEST_ID3        (SHARP_MP_ID3)

enum SHARP_SAMPLE_CONFIG_INDEX {
    NOT_DEFINED_INDEX = 0,
    BOOT_ON_60HZ_INDEX,
    SAMPLE_PRE_TS_INDEX,
    SAMPLE_TS_INDEX,
    SAMPLE_PRE_ES_INDEX,
    SAMPLE_ES_INDEX,
    SAMPLE_ES2_1_INDEX,
    SAMPLE_ES2_2_INDEX,
    SAMPLE_ES3_INDEX,
    SAMPLE_ES2_FINAL_INDEX,
    SAMPLE_CS_INDEX,
    SAMPLE_MP_INDEX,
    PARAMS_DSC_INDEX,
    FPS_120_VFP_INDEX,
    FPS_90_VFP_INDEX,
    FPS_72_VFP_INDEX,
    FPS_60_VFP_INDEX,
    FPS_45_VFP_INDEX,
    FPS_30_VFP_INDEX,
    FPS_24_VFP_INDEX,
    FPS_15_VFP_INDEX,
    FPS_10_VFP_INDEX,
    FPS_6_VFP_INDEX,
    FPS_1_VFP_FRAMESKIP_INDEX,
    FPS_120_FRAMESKIP_INDEX,
    FPS_60_FRAMESKIP_INDEX,
    FPS_30_FRAMESKIP_INDEX,
    FPS_10_FRAMESKIP_INDEX,
    FPS_1_FRAMESKIP_INDEX,
};

enum SHARP_SAMPLE_CONFIG {
    NOT_DEFINED             = 0,
    BOOT_ON_60HZ            = BIT(BOOT_ON_60HZ_INDEX),
    SAMPLE_PRE_TS           = BIT(SAMPLE_PRE_TS_INDEX),
    SAMPLE_TS               = BIT(SAMPLE_TS_INDEX),
    SAMPLE_PRE_ES           = BIT(SAMPLE_PRE_ES_INDEX),
    SAMPLE_ES               = BIT(SAMPLE_ES_INDEX),
    SAMPLE_ES2_1            = BIT(SAMPLE_ES2_1_INDEX),
    SAMPLE_ES2_2            = BIT(SAMPLE_ES2_2_INDEX),
    SAMPLE_ES3              = BIT(SAMPLE_ES3_INDEX),
    SAMPLE_ES2_FINAL        = BIT(SAMPLE_ES2_FINAL_INDEX),
    SAMPLE_CS               = BIT(SAMPLE_CS_INDEX),
    SAMPLE_MP               = BIT(SAMPLE_MP_INDEX),
    PARAMS_DSC              = BIT(PARAMS_DSC_INDEX),
    FPS_120_VFP             = BIT(FPS_120_VFP_INDEX),
    FPS_90_VFP              = BIT(FPS_90_VFP_INDEX),
    FPS_72_VFP              = BIT(FPS_72_VFP_INDEX),
    FPS_60_VFP              = BIT(FPS_60_VFP_INDEX),
    FPS_45_VFP              = BIT(FPS_45_VFP_INDEX),
    FPS_30_VFP              = BIT(FPS_30_VFP_INDEX),
    FPS_24_VFP              = BIT(FPS_24_VFP_INDEX),
    FPS_15_VFP              = BIT(FPS_15_VFP_INDEX),
    FPS_10_VFP              = BIT(FPS_10_VFP_INDEX),
    FPS_6_VFP               = BIT(FPS_6_VFP_INDEX),
    FPS_1_VFP_FRAMESKIP     = BIT(FPS_1_VFP_FRAMESKIP_INDEX),
    FPS_120_FRAMESKIP       = BIT(FPS_120_FRAMESKIP_INDEX),
    FPS_60_FRAMESKIP        = BIT(FPS_60_FRAMESKIP_INDEX),
    FPS_30_FRAMESKIP        = BIT(FPS_30_FRAMESKIP_INDEX),
    FPS_10_FRAMESKIP        = BIT(FPS_10_FRAMESKIP_INDEX),
    FPS_1_FRAMESKIP         = BIT(FPS_1_FRAMESKIP_INDEX),
};
#define SAMPLE_MASK                (SAMPLE_PRE_TS | SAMPLE_TS | SAMPLE_PRE_ES | SAMPLE_ES | SAMPLE_ES2_1 | SAMPLE_ES2_2 | SAMPLE_ES3 | SAMPLE_ES2_FINAL | SAMPLE_CS | SAMPLE_MP)
#define FPS_CONFIG_VFP_ONLY        (FPS_120_VFP | FPS_90_VFP | FPS_72_VFP | FPS_60_VFP | FPS_45_VFP | FPS_30_VFP | FPS_24_VFP | FPS_15_VFP | FPS_10_VFP | FPS_6_VFP)
#define FPS_CONFIG_FRAMESKIP_ONLY  (FPS_120_FRAMESKIP | FPS_60_FRAMESKIP | FPS_30_FRAMESKIP | FPS_10_FRAMESKIP | FPS_1_FRAMESKIP)


#define PRE_TS_CONFIG       (SAMPLE_PRE_TS      | PARAMS_DSC | BOOT_ON_60HZ | FPS_120_VFP | FPS_60_VFP)
#define PRE_TS_NAME         "Pre-TS"
#define TS_CONFIG           (SAMPLE_TS          | PARAMS_DSC | BOOT_ON_60HZ | FPS_CONFIG_VFP_ONLY)
#define TS_NAME             "TS"
#define PRE_ES_CONFIG       (SAMPLE_PRE_ES      | PARAMS_DSC | BOOT_ON_60HZ | FPS_CONFIG_VFP_ONLY)
#define PRE_ES_NAME         "Pre-ES"
#define ES_CONFIG           (SAMPLE_ES          | PARAMS_DSC | BOOT_ON_60HZ | FPS_CONFIG_VFP_ONLY)
#define ES_NAME             "ES"
#define ES2_1_CONFIG        (SAMPLE_ES2_1       | PARAMS_DSC | BOOT_ON_60HZ | FPS_CONFIG_VFP_ONLY)
#define ES2_1_NAME          "ES2-1"
#define ES2_2_CONFIG        (SAMPLE_ES2_2       | PARAMS_DSC | BOOT_ON_60HZ | FPS_CONFIG_VFP_ONLY)
#define ES2_2_NAME          "ES2-2"
#define ES3_CONFIG          (SAMPLE_ES3         | PARAMS_DSC | BOOT_ON_60HZ | FPS_CONFIG_VFP_ONLY)
#define ES3_NAME            "ES3"
#define ES2_FINAL_CONFIG    (SAMPLE_ES2_FINAL   | PARAMS_DSC | BOOT_ON_60HZ | FPS_CONFIG_VFP_ONLY)
#define ES2_FINAL_NAME      "ES2_final"
#define CS_CONFIG           (SAMPLE_CS          | PARAMS_DSC | BOOT_ON_60HZ | FPS_CONFIG_VFP_ONLY)
#define CS_NAME             "CS"
#define MP_CONFIG           (SAMPLE_MP          | PARAMS_DSC | BOOT_ON_60HZ | FPS_CONFIG_VFP_ONLY)
#define MP_NAME             "MP"
#define LATEST_CONFIG       (MP_CONFIG)
#define LATEST_NAME         "!!!Newer version!!! we use "MP_NAME
#define UNKNOWN_CONFIG      (TS_CONFIG)
#define UNKNOWN_NAME        "!!!Unknown version!!! we use "TS_NAME
#define NOT_FOUND_CONFIG    (MP_CONFIG)
#define NOT_FOUND_NAME      "!!!Not Found!!! we use "MP_NAME


// #define SW_ROUNDED_CORNER_SUPPORT
// #define CONFIG_MTK_LCM_PHYSICAL_ROTATION_HW
#define USE_DSC
#ifdef CONFIG_DRM_MTK_ICOM_ENABLE_LOW_POWER_IDLE_MODE
    #define USE_DISP_IDLE
#endif
// #define USE_LFR
// #define USE_LP_PERLINE
#define USE_DSI_CLOCK_NON_CONTINUOUS
#define USE_ESD_CHECK
// #define USE_CUST_ESD

#define SHARP_NT36523N_COMPATIBLE "sharp,nt36523n,vdo,120hz"

#define VFP_CASE        (FPS_CONFIG_VFP_ONLY | FPS_1_VFP_FRAMESKIP)
#define FRAMESKIP_CASE  (FPS_CONFIG_FRAMESKIP_ONLY | FPS_1_VFP_FRAMESKIP)

#endif //end of _NT36523N_UXGA_SHARP_H_
