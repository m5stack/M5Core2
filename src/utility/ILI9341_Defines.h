// Change the width and height if required (defined in portrait mode)
// or use the constructor to over-ride defaults
#define TFT_WIDTH  240
#define TFT_HEIGHT 320


// Color definitions for backwards compatibility with old sketches
// use colour definitions like TFT_BLACK to make sketches more portable
#define ILI9341_BLACK       0x0000      /*   0,   0,   0 */
#define ILI9341_NAVY        0x000F      /*   0,   0, 128 */
#define ILI9341_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9341_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define ILI9341_MAROON      0x7800      /* 128,   0,   0 */
#define ILI9341_PURPLE      0x780F      /* 128,   0, 128 */
#define ILI9341_OLIVE       0x7BE0      /* 128, 128,   0 */
#define ILI9341_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define ILI9341_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ILI9341_BLUE        0x001F      /*   0,   0, 255 */
#define ILI9341_GREEN       0x07E0      /*   0, 255,   0 */
#define ILI9341_CYAN        0x07FF      /*   0, 255, 255 */
#define ILI9341_RED         0xF800      /* 255,   0,   0 */
#define ILI9341_MAGENTA     0xF81F      /* 255,   0, 255 */
#define ILI9341_YELLOW      0xFFE0      /* 255, 255,   0 */
#define ILI9341_WHITE       0xFFFF      /* 255, 255, 255 */
#define ILI9341_ORANGE      0xFD20      /* 255, 165,   0 */
#define ILI9341_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ILI9341_PINK        0xF81F

#define BLACK               0x0000      /*   0,   0,   0 */
#define NAVY                0x000F      /*   0,   0, 128 */
#define DARKGREEN           0x03E0      /*   0, 128,   0 */
#define DARKCYAN            0x03EF      /*   0, 128, 128 */
#define MAROON              0x7800      /* 128,   0,   0 */
#define PURPLE              0x780F      /* 128,   0, 128 */
#define OLIVE               0x7BE0      /* 128, 128,   0 */
#define LIGHTGREY           0xC618      /* 192, 192, 192 */
#define DARKGREY            0x7BEF      /* 128, 128, 128 */
#define BLUE                0x001F      /*   0,   0, 255 */
#define GREEN               0x07E0      /*   0, 255,   0 */
#define CYAN                0x07FF      /*   0, 255, 255 */
#define RED                 0xF800      /* 255,   0,   0 */
#define MAGENTA             0xF81F      /* 255,   0, 255 */
#define YELLOW              0xFFE0      /* 255, 255,   0 */
#define WHITE               0xFFFF      /* 255, 255, 255 */
#define ORANGE              0xFD20      /* 255, 165,   0 */
#define GREENYELLOW         0xAFE5      /* 173, 255,  47 */
#define PINK                0xF81F  

#define ALICEBLUE             0xF7DF 
#define ANTIQUEWHITE          0xFF5A 
#define AQUA                  0x07FF 
#define AQUAMARINE            0x7FFA 
#define AZURE                 0xF7FF 
#define BEIGE                 0xF7BB 
#define BISQUE                0xFF38 
#define BLANCHEDALMOND        0xFF59 
#define BLUEVIOLET            0x895C 
#define BROWN                 0xA145 
#define BURLYWOOD             0xDDD0 
#define CADETBLUE             0x5CF4 
#define CHARTREUSE            0x7FE0 
#define CHOCOLATE             0xD343 
#define CORAL                 0xFBEA 
#define CORNFLOWERBLUE        0x64BD 
#define CORNSILK              0xFFDB 
#define CRIMSON               0xD8A7 
#define DARKBLUE              0x0011 
#define DARKGOLDENROD         0xBC21 
#define DARKKHAKI             0xBDAD 
#define DARKMAGENTA           0x8811 
#define DARKOLIVEGREEN        0x5345 
#define DARKORANGE            0xFC60 
#define DARKORCHID            0x9999 
#define DARKRED               0x8800 
#define DARKSALMON            0xECAF 
#define DARKSEAGREEN          0x8DF1 
#define DARKSLATEBLUE         0x49F1 
#define DARKSLATEGRAY         0x2A69 
#define DARKTURQUOISE         0x067A 
#define DARKVIOLET            0x901A 
#define DEEPPINK              0xF8B2 
#define DEEPSKYBLUE           0x05FF 
#define DIMGRAY               0x6B4D 
#define DODGERBLUE            0x1C9F 
#define FIREBRICK             0xB104 
#define FLORALWHITE           0xFFDE 
#define FORESTGREEN           0x2444 
#define FUCHSIA               0xF81F 
#define GAINSBORO             0xDEFB 
#define GHOSTWHITE            0xFFDF 
#define GOLD                  0xFEA0 
#define GOLDENROD             0xDD24 
#define GRAY                  0x8410 
#define HONEYDEW              0xF7FE 
#define HOTPINK               0xFB56 
#define INDIANRED             0xCAEB 
#define INDIGO                0x4810 
#define IVORY                 0xFFFE 
#define KHAKI                 0xF731 
#define LAVENDER              0xE73F 
#define LAVENDERBLUSH         0xFF9E 
#define LAWNGREEN             0x7FE0 
#define LEMONCHIFFON          0xFFD9 
#define LIGHTBLUE             0xAEDC 
#define LIGHTCORAL            0xF410 
#define LIGHTCYAN             0xE7FF 
#define LIGHTGOLDENRODYELLOW  0xFFDA 
#define LIGHTGREEN            0x9772 
#define LIGHTPINK             0xFDB8 
#define LIGHTSALMON           0xFD0F 
#define LIGHTSEAGREEN         0x2595 
#define LIGHTSKYBLUE          0x867F 
#define LIGHTSLATEGRAY        0x7453 
#define LIGHTSTEELBLUE        0xB63B 
#define LIGHTYELLOW           0xFFFC 
#define LIME                  0x07E0 
#define LIMEGREEN             0x3666 
#define LINEN                 0xFF9C 
#define MEDIUMAQUAMARINE      0x6675 
#define MEDIUMBLUE            0x0019 
#define MEDIUMORCHID          0xBABA 
#define MEDIUMPURPLE          0x939B 
#define MEDIUMSEAGREEN        0x3D8E 
#define MEDIUMSLATEBLUE       0x7B5D 
#define MEDIUMSPRINGGREEN     0x07D3 
#define MEDIUMTURQUOISE       0x4E99 
#define MEDIUMVIOLETRED       0xC0B0 
#define MIDNIGHTBLUE          0x18CE 
#define MINTCREAM             0xF7FF 
#define MISTYROSE             0xFF3C 
#define MOCCASIN              0xFF36 
#define NAVAJOWHITE           0xFEF5 
#define OLDLACE               0xFFBC 
#define OLIVEDRAB             0x6C64 
#define ORANGERED             0xFA20 
#define ORCHID                0xDB9A 
#define PALEGOLDENROD         0xEF55 
#define PALEGREEN             0x9FD3 
#define PALETURQUOISE         0xAF7D 
#define PALEVIOLETRED         0xDB92 
#define PAPAYAWHIP            0xFF7A 
#define PEACHPUFF             0xFED7 
#define PERU                  0xCC27 
#define PLUM                  0xDD1B 
#define POWDERBLUE            0xB71C 
#define ROSYBROWN             0xBC71 
#define ROYALBLUE             0x435C 
#define SADDLEBROWN           0x8A22 
#define SALMON                0xFC0E 
#define SANDYBROWN            0xF52C 
#define SEAGREEN              0x2C4A 
#define SEASHELL              0xFFBD 
#define SIENNA                0xA285 
#define SILVER                0xC618 
#define SKYBLUE               0x867D 
#define SLATEBLUE             0x6AD9 
#define SLATEGRAY             0x7412 
#define SNOW                  0xFFDF 
#define SPRINGGREEN           0x07EF 
#define STEELBLUE             0x4416 
#define TAN                   0xD5B1 
#define TEAL                  0x0410 
#define THISTLE               0xDDFB 
#define TOMATO                0xFB08 
#define TURQUOISE             0x471A 
#define VIOLET                0xEC1D 
#define WHEAT                 0xF6F6 
#define WHITESMOKE            0xF7BE 
#define YELLOWGREEN           0x9E66

// Delay between some initialisation commands
#define TFT_INIT_DELAY 0x80 // Not used unless commandlist invoked


// Generic commands used by TFT_eSPI.cpp
#define TFT_NOP     0x00
#define TFT_SWRST   0x01

#define TFT_CASET   0x2A
#define TFT_PASET   0x2B
#define TFT_RAMWR   0x2C

#define TFT_RAMRD   0x2E
#define TFT_IDXRD   0xDD // ILI9341 only, indexed control register read

#define TFT_MADCTL  0x36
#define TFT_MAD_MY  0x80
#define TFT_MAD_MX  0x40
#define TFT_MAD_MV  0x20
#define TFT_MAD_ML  0x10
#define TFT_MAD_BGR 0x08
#define TFT_MAD_MH  0x04
#define TFT_MAD_RGB 0x00

#ifdef TFT_RGB_ORDER
  #if (TFT_RGB_ORDER == 1)
    #define TFT_MAD_COLOR_ORDER TFT_MAD_RGB
  #else
    #define TFT_MAD_COLOR_ORDER TFT_MAD_BGR
  #endif
#else
  #define TFT_MAD_COLOR_ORDER TFT_MAD_BGR
#endif

#define TFT_INVOFF  0x20
#define TFT_INVON   0x21


// All ILI9341 specific commands some are used by init()
#define ILI9341_NOP     0x00
#define ILI9341_SWRESET 0x01
#define ILI9341_RDDID   0x04
#define ILI9341_RDDST   0x09

#define ILI9341_SLPIN   0x10
#define ILI9341_SLPOUT  0x11
#define ILI9341_PTLON   0x12
#define ILI9341_NORON   0x13

#define ILI9341_RDMODE  0x0A
#define ILI9341_RDMADCTL  0x0B
#define ILI9341_RDPIXFMT  0x0C
#define ILI9341_RDIMGFMT  0x0A
#define ILI9341_RDSELFDIAG  0x0F

#define ILI9341_INVOFF  0x20
#define ILI9341_INVON   0x21
#define ILI9341_GAMMASET 0x26
#define ILI9341_DISPOFF 0x28
#define ILI9341_DISPON  0x29

#define ILI9341_CASET   0x2A
#define ILI9341_PASET   0x2B
#define ILI9341_RAMWR   0x2C
#define ILI9341_RAMRD   0x2E

#define ILI9341_PTLAR   0x30
#define ILI9341_VSCRDEF 0x33
#define ILI9341_MADCTL  0x36
#define ILI9341_VSCRSADD 0x37
#define ILI9341_PIXFMT  0x3A

#define ILI9341_WRDISBV  0x51
#define ILI9341_RDDISBV  0x52
#define ILI9341_WRCTRLD  0x53

#define ILI9341_FRMCTR1 0xB1
#define ILI9341_FRMCTR2 0xB2
#define ILI9341_FRMCTR3 0xB3
#define ILI9341_INVCTR  0xB4
#define ILI9341_DFUNCTR 0xB6

#define ILI9341_PWCTR1  0xC0
#define ILI9341_PWCTR2  0xC1
#define ILI9341_PWCTR3  0xC2
#define ILI9341_PWCTR4  0xC3
#define ILI9341_PWCTR5  0xC4
#define ILI9341_VMCTR1  0xC5
#define ILI9341_VMCTR2  0xC7

#define ILI9341_RDID4   0xD3
#define ILI9341_RDINDEX 0xD9
#define ILI9341_RDID1   0xDA
#define ILI9341_RDID2   0xDB
#define ILI9341_RDID3   0xDC
#define ILI9341_RDIDX   0xDD // TBC

#define ILI9341_GMCTRP1 0xE0
#define ILI9341_GMCTRN1 0xE1

#define ILI9341_MADCTL_MY  0x80
#define ILI9341_MADCTL_MX  0x40
#define ILI9341_MADCTL_MV  0x20
#define ILI9341_MADCTL_ML  0x10
#define ILI9341_MADCTL_RGB 0x00
#define ILI9341_MADCTL_BGR 0x08
#define ILI9341_MADCTL_MH  0x04
