#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <map>
#include <FastLED_NeoMatrix.h>
#include <Globals.h>

std::map<char, uint16_t> CharMap = {
    {32, 2}, {33, 2}, {34, 4}, {35, 4}, {36, 4}, {37, 4}, {38, 4}, {39, 2}, {40, 3}, {41, 3}, {42, 4}, {43, 4}, {44, 3}, {45, 4}, {46, 2}, {47, 4}, {48, 4}, {49, 4}, {50, 4}, {51, 4}, {52, 4}, {53, 4}, {54, 4}, {55, 4}, {56, 4}, {57, 4}, {58, 2}, {59, 3}, {60, 4}, {61, 4}, {62, 4}, {63, 4}, {64, 4}, {65, 4}, {66, 4}, {67, 4}, {68, 4}, {69, 4}, {70, 4}, {71, 4}, {72, 4}, {73, 2}, {74, 4}, {75, 4}, {76, 4}, {77, 6}, {78, 5}, {79, 4}, {80, 4}, {81, 5}, {82, 4}, {83, 4}, {84, 4}, {85, 4}, {86, 4}, {87, 6}, {88, 4}, {89, 4}, {90, 4}, {91, 4}, {92, 4}, {93, 4}, {94, 4}, {95, 4}, {96, 3}, {97, 4}, {98, 4}, {99, 4}, {100, 4}, {101, 4}, {102, 4}, {103, 4}, {104, 4}, {105, 2}, {106, 4}, {107, 4}, {108, 4}, {109, 4}, {110, 4}, {111, 4}, {112, 4}, {113, 4}, {114, 4}, {115, 4}, {116, 4}, {117, 4}, {118, 4}, {119, 4}, {120, 4}, {121, 4}, {122, 4}, {123, 4}, {124, 2}, {125, 4}, {126, 4}, {161, 2}, {162, 4}, {163, 4}, {164, 4}, {165, 4}, {166, 2}, {167, 4}, {168, 4}, {169, 4}, {170, 4}, {171, 3}, {172, 4}, {173, 3}, {174, 4}, {175, 4}, {176, 3}, {177, 4}, {178, 4}, {179, 4}, {180, 3}, {181, 4}, {182, 4}, {183, 4}, {184, 4}, {185, 2}, {186, 4}, {187, 3}, {188, 4}, {189, 4}, {190, 4}, {191, 4}, {192, 4}, {193, 4}, {194, 4}, {195, 4}, {196, 4}, {197, 4}, {198, 4}, {199, 4}, {200, 4}, {201, 4}, {202, 4}, {203, 4}, {204, 4}, {205, 4}, {206, 4}, {207, 4}, {208, 4}, {209, 4}, {210, 4}, {211, 4}, {212, 4}, {213, 4}, {214, 4}, {215, 4}, {216, 4}, {217, 4}, {218, 4}, {219, 4}, {220, 4}, {221, 4}, {222, 4}, {223, 4}, {224, 4}, {225, 4}, {226, 4}, {227, 4}, {228, 4}, {229, 4}, {230, 4}, {231, 4}, {232, 4}, {233, 4}, {234, 4}, {235, 4}, {236, 3}, {237, 3}, {238, 4}, {239, 4}, {240, 4}, {241, 4}, {242, 4}, {243, 4}, {244, 4}, {245, 4}, {246, 4}, {247, 4}, {248, 4}, {249, 4}, {250, 4}, {251, 4}, {252, 4}, {253, 4}, {254, 4}, {255, 4}, {285, 2}, {338, 4}, {339, 4}, {352, 4}, {353, 4}, {376, 4}, {381, 4}, {382, 4}, {3748, 2}, {5024, 2}, {8226, 2}, {8230, 4}, {8364, 4}, {65533, 4}};

CRGB kelvinToRGB(int kelvin)
{
    float temperature = kelvin / 100.0;
    float red, green, blue;

    if (temperature <= 66)
    {
        red = 255;
        green = temperature;
        green = 99.4708025861 * log(green) - 161.1195681661;
    }
    else
    {
        red = temperature - 60;
        red = 329.698727446 * pow(red, -0.1332047592);
        green = temperature - 60;
        green = 288.1221695283 * pow(green, -0.0755148492);
    }

    if (temperature >= 66)
    {
        blue = 255;
    }
    else if (temperature <= 19)
    {
        blue = 0;
    }
    else
    {
        blue = temperature - 10;
        blue = 138.5177312231 * log(blue) - 305.0447927307;
    }

    red = constrain(red, 0, 255);
    green = constrain(green, 0, 255);
    blue = constrain(blue, 0, 255);

    return CRGB((uint8_t)red, (uint8_t)green, (uint8_t)blue);
}

uint32_t hsvToRgb(uint8_t h, uint8_t s, uint8_t v)
{
    CHSV hsv(h, s, v);
    CRGB rgb;
    hsv2rgb_spectrum(hsv, rgb);
    return ((uint32_t)rgb.r << 16) | 
           ((uint32_t)rgb.g << 8)  | 
           (uint32_t)rgb.b;
}


uint32_t hexToUint32(const char *hexString)
{
    uint32_t rgbValue = (uint32_t)strtol(hexString, NULL, 16);
    return rgbValue; // Nur RGB, kein Alpha-Kanal
}


uint32_t getColorFromJsonVariant(JsonVariant colorVariant, uint32_t defaultColor)
{
    if (colorVariant.is<String>())
    {
        String hexString = colorVariant.as<String>();
        hexString.replace("#", "");
        return hexToUint32(hexString.c_str());
    }
    else if (colorVariant.is<JsonArray>())
    {
        JsonArray colorArray = colorVariant.as<JsonArray>();
        if (colorArray.size() == 3) // RGB
        {
            uint8_t r = colorArray[0];
            uint8_t g = colorArray[1];
            uint8_t b = colorArray[2];
            return (r << 16) | (g << 8) | b; // Ignoring alpha channel
        }
        else if (colorArray.size() == 4 && colorArray[0] == "HSV") // HSV
        {
            uint8_t h = colorArray[1];
            uint8_t s = colorArray[2];
            uint8_t v = colorArray[3];
            // Assuming you have a function to convert HSV to RGB
            uint8_t r, g, b;
            // hsv2rgb(h, s, v, r, g, b);
            return (r << 16) | (g << 8) | b; // Ignoring alpha channel
        }
    }

    return defaultColor;
}

double roundToDecimalPlaces(double value, int places)
{
  double factor = pow(10.0, places);
  return round(value * factor) / factor;
}

float getTextWidth(const char *text, byte textCase)
{
    float width = 0;
    for (const char *c = text; *c != '\0'; ++c)
    {
        char current_char = *c;
        if ((UPPERCASE_LETTERS && textCase == 0) || textCase == 1)
        {
            current_char = toupper(current_char);
        }
        if (CharMap.count(current_char) > 0)
        {
            width += CharMap[current_char];
        }
        else
        {
            width += 4;
        }
    }
    return width;
}

static byte c1;
byte utf8ascii(byte ascii)
{
    if (ascii < 128)
    {
        c1 = 0;
        return (ascii);
    }
    byte last = c1;
    c1 = ascii;
    switch (last)
    {
    case 0xC2:
        return (ascii)-34;
        break;
    case 0xC3:
        return (ascii | 0xC0) - 34;
        break;
    case 0x82:
        if (ascii == 0xAC)
            return (0xEA);
    }
    return (0);
}

String utf8ascii(String s)
{
    String r = "";
    char c;
    for (unsigned int i = 0; i < s.length(); i++)
    {
        c = utf8ascii(s.charAt(i));
        if (c != 0)
            r += c;
    }
    return r;
}
#endif