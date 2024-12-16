#include "Global.h"
#include "classes/IoTItem.h"
#include "FastLED.h"
#include "effects.h"
#include "matrix.h"
extern IoTGpio IoTgpio;

class GyverLAMP : public IoTItem
{
private:
    unsigned long prevmillis = 0;
    String curEffect;

public:
    //=======================================================================================================
    // setup()
    GyverLAMP(String parameters) : IoTItem(parameters)
    {
        jsonRead(parameters, F("speed"), _speed);
        jsonRead(parameters, F("dalay"), _dalay);
        jsonRead(parameters, F("brightness"), _brightness);
        //    jsonRead(parameters, F("x_size"), _x_size);
        //    jsonRead(parameters, F("y_size"), _y_size);

        LEDS.setBrightness(_brightness);                     // ограничить максимальную яркость
        LEDS.addLeds<WS2812B, LED_DT, GRB>(leds, LED_COUNT); // настрйоки для нашей ленты (ленты на WS2811, WS2812, WS2812B)
        one_color_all(0, 0, 0);                              // погасить все светодиоды
        LEDS.show();
    }

    //=======================================================================================================
    // doByInterval()
    void doByInterval()
    {
    }

    //=======================================================================================================
    // loop()
    void loop()
    {

        // yield();

        if (millis() - prevmillis > _dalay)
        {

            prevmillis = millis();
            thisdelay = _speed;

            if (curEffect == "rainbow_fade")
            {
                // rainbow_fade();
                ihue++;
                if (ihue > 255)
                {
                    ihue = 0;
                }
                for (int idex = 0; idex < LED_COUNT; idex++)
                {
                    leds[idex] = CHSV(ihue, thissat, 255);
                }
                LEDS.show();
            }
            if (curEffect == "rainbow_loop")
            {
                // rainbow_loop();
                idex++;
                ihue = ihue + thisstep;
                if (idex >= LED_COUNT)
                {
                    idex = 0;
                }
                if (ihue > 255)
                {
                    ihue = 0;
                }
                leds[idex] = CHSV(ihue, thissat, 255);
                LEDS.show();
            }
            if (curEffect == "random_burst")
            {
                // random_burst();
                idex = random(0, LED_COUNT);
                ihue = random(0, 255);
                leds[idex] = CHSV(ihue, thissat, 255);
                LEDS.show();
            }
            if (curEffect == "color_bounce")
            {
                // color_bounce();
                if (bouncedirection == 0)
                {
                    idex = idex + 1;
                    if (idex == LED_COUNT)
                    {
                        bouncedirection = 1;
                        idex = idex - 1;
                    }
                }
                if (bouncedirection == 1)
                {
                    idex = idex - 1;
                    if (idex == 0)
                    {
                        bouncedirection = 0;
                    }
                }
                for (int i = 0; i < LED_COUNT; i++)
                {
                    if (i == idex)
                    {
                        leds[i] = CHSV(thishue, thissat, 255);
                    }
                    else
                    {
                        leds[i] = CHSV(0, 0, 0);
                    }
                }
                LEDS.show();
            }
            if (curEffect == "color_bounceFADE")
            {
                // color_bounceFADE();
                if (bouncedirection == 0)
                {
                    idex = idex + 1;
                    if (idex == LED_COUNT)
                    {
                        bouncedirection = 1;
                        idex = idex - 1;
                    }
                }
                if (bouncedirection == 1)
                {
                    idex = idex - 1;
                    if (idex == 0)
                    {
                        bouncedirection = 0;
                    }
                }
                int iL1 = adjacent_cw(idex);
                int iL2 = adjacent_cw(iL1);
                int iL3 = adjacent_cw(iL2);
                int iR1 = adjacent_ccw(idex);
                int iR2 = adjacent_ccw(iR1);
                int iR3 = adjacent_ccw(iR2);
                for (int i = 0; i < LED_COUNT; i++)
                {
                    if (i == idex)
                    {
                        leds[i] = CHSV(thishue, thissat, 255);
                    }
                    else if (i == iL1)
                    {
                        leds[i] = CHSV(thishue, thissat, 150);
                    }
                    else if (i == iL2)
                    {
                        leds[i] = CHSV(thishue, thissat, 80);
                    }
                    else if (i == iL3)
                    {
                        leds[i] = CHSV(thishue, thissat, 20);
                    }
                    else if (i == iR1)
                    {
                        leds[i] = CHSV(thishue, thissat, 150);
                    }
                    else if (i == iR2)
                    {
                        leds[i] = CHSV(thishue, thissat, 80);
                    }
                    else if (i == iR3)
                    {
                        leds[i] = CHSV(thishue, thissat, 20);
                    }
                    else
                    {
                        leds[i] = CHSV(0, 0, 0);
                    }
                }
                LEDS.show();
            }
            if (curEffect == "ems_lightsONE")
            {
                // ems_lightsONE();
                idex++;
                if (idex >= LED_COUNT)
                {
                    idex = 0;
                }
                int idexR = idex;
                int idexB = antipodal_index(idexR);
                int thathue = (thishue + 160) % 255;
                for (int i = 0; i < LED_COUNT; i++)
                {
                    if (i == idexR)
                    {
                        leds[i] = CHSV(thishue, thissat, 255);
                    }
                    else if (i == idexB)
                    {
                        leds[i] = CHSV(thathue, thissat, 255);
                    }
                    else
                    {
                        leds[i] = CHSV(0, 0, 0);
                    }
                }
                LEDS.show();
            }
            if (curEffect == "ems_lightsALL")
            {
                // ems_lightsALL();
                idex++;
                if (idex >= LED_COUNT)
                {
                    idex = 0;
                }
                int idexR = idex;
                int idexB = antipodal_index(idexR);
                int thathue = (thishue + 160) % 255;
                leds[idexR] = CHSV(thishue, thissat, 255);
                leds[idexB] = CHSV(thathue, thissat, 255);
                LEDS.show();
            }
            if (curEffect == "flicker")
            {
                flicker();
            }

            if (curEffect == "pulse_one_color_all")
            {
                // pulse_one_color_all();
                if (bouncedirection == 0)
                {
                    ibright++;
                    if (ibright >= 255)
                    {
                        bouncedirection = 1;
                    }
                }
                if (bouncedirection == 1)
                {
                    ibright = ibright - 1;
                    if (ibright <= 1)
                    {
                        bouncedirection = 0;
                    }
                }
                for (int idex = 0; idex < LED_COUNT; idex++)
                {
                    leds[idex] = CHSV(thishue, thissat, ibright);
                }
                LEDS.show();
            }
            if (curEffect == "pulse_one_color_all_rev")
            {
                // pulse_one_color_all_rev();
                if (bouncedirection == 0)
                {
                    isat++;
                    if (isat >= 255)
                    {
                        bouncedirection = 1;
                    }
                }
                if (bouncedirection == 1)
                {
                    isat = isat - 1;
                    if (isat <= 1)
                    {
                        bouncedirection = 0;
                    }
                }
                for (int idex = 0; idex < LED_COUNT; idex++)
                {
                    leds[idex] = CHSV(thishue, isat, 255);
                }
                LEDS.show();
            }
            if (curEffect == "fade_vertical")
            {
                // fade_vertical();
                idex++;
                if (idex > TOP_INDEX)
                {
                    idex = 0;
                }
                int idexA = idex;
                int idexB = horizontal_index(idexA);
                ibright = ibright + 10;
                if (ibright > 255)
                {
                    ibright = 0;
                }
                leds[idexA] = CHSV(thishue, thissat, ibright);
                leds[idexB] = CHSV(thishue, thissat, ibright);
                LEDS.show();
            }
            if (curEffect == "rule30")
            {
                // rule30();
                if (bouncedirection == 0)
                {
                    random_red();
                    bouncedirection = 1;
                }
                copy_led_array();
                int iCW;
                int iCCW;
                int y = 100;
                for (int i = 0; i < LED_COUNT; i++)
                {
                    iCW = adjacent_cw(i);
                    iCCW = adjacent_ccw(i);
                    if (ledsX[iCCW][0] > y && ledsX[i][0] > y && ledsX[iCW][0] > y)
                    {
                        leds[i].r = 0;
                    }
                    if (ledsX[iCCW][0] > y && ledsX[i][0] > y && ledsX[iCW][0] <= y)
                    {
                        leds[i].r = 0;
                    }
                    if (ledsX[iCCW][0] > y && ledsX[i][0] <= y && ledsX[iCW][0] > y)
                    {
                        leds[i].r = 0;
                    }
                    if (ledsX[iCCW][0] > y && ledsX[i][0] <= y && ledsX[iCW][0] <= y)
                    {
                        leds[i].r = 255;
                    }
                    if (ledsX[iCCW][0] <= y && ledsX[i][0] > y && ledsX[iCW][0] > y)
                    {
                        leds[i].r = 255;
                    }
                    if (ledsX[iCCW][0] <= y && ledsX[i][0] > y && ledsX[iCW][0] <= y)
                    {
                        leds[i].r = 255;
                    }
                    if (ledsX[iCCW][0] <= y && ledsX[i][0] <= y && ledsX[iCW][0] > y)
                    {
                        leds[i].r = 255;
                    }
                    if (ledsX[iCCW][0] <= y && ledsX[i][0] <= y && ledsX[iCW][0] <= y)
                    {
                        leds[i].r = 0;
                    }
                }
                LEDS.show();
            }
            if (curEffect == "random_march")
            {
                // random_march();
                copy_led_array();
                int iCCW;
                leds[0] = CHSV(random(0, 255), 255, 255);
                for (int idex = 1; idex < LED_COUNT; idex++)
                {
                    iCCW = adjacent_ccw(idex);
                    leds[idex].r = ledsX[iCCW][0];
                    leds[idex].g = ledsX[iCCW][1];
                    leds[idex].b = ledsX[iCCW][2];
                }
                LEDS.show();
            }
            if (curEffect == "rwb_march")
            {
                // rwb_march();
                copy_led_array();
                int iCCW;
                idex++;
                if (idex > 2)
                {
                    idex = 0;
                }
                switch (idex)
                {
                case 0:
                    leds[0].r = 255;
                    leds[0].g = 0;
                    leds[0].b = 0;
                    break;
                case 1:
                    leds[0].r = 255;
                    leds[0].g = 255;
                    leds[0].b = 255;
                    break;
                case 2:
                    leds[0].r = 0;
                    leds[0].g = 0;
                    leds[0].b = 255;
                    break;
                }
                for (int i = 1; i < LED_COUNT; i++)
                {
                    iCCW = adjacent_ccw(i);
                    leds[i].r = ledsX[iCCW][0];
                    leds[i].g = ledsX[iCCW][1];
                    leds[i].b = ledsX[iCCW][2];
                }
                LEDS.show();
            }
            if (curEffect == "radiation")
            {
                // radiation();
                int N3 = int(LED_COUNT / 3);
                int N6 = int(LED_COUNT / 6);
                int N12 = int(LED_COUNT / 12);
                for (int i = 0; i < N6; i++)
                { //-HACKY, I KNOW...
                    tcount = tcount + .02;
                    if (tcount > 3.14)
                    {
                        tcount = 0.0;
                    }
                    ibright = int(sin(tcount) * 255);
                    int j0 = (i + LED_COUNT - N12) % LED_COUNT;
                    int j1 = (j0 + N3) % LED_COUNT;
                    int j2 = (j1 + N3) % LED_COUNT;
                    leds[j0] = CHSV(thishue, thissat, ibright);
                    leds[j1] = CHSV(thishue, thissat, ibright);
                    leds[j2] = CHSV(thishue, thissat, ibright);
                }
                LEDS.show();
            }
            if (curEffect == "color_loop_vardelay")
            {
                // color_loop_vardelay();
                idex++;
                if (idex > LED_COUNT)
                {
                    idex = 0;
                }
                int di = abs(TOP_INDEX - idex);
                int t = constrain((10 / di) * 10, 10, 500);
                for (int i = 0; i < LED_COUNT; i++)
                {
                    if (i == idex)
                    {
                        leds[i] = CHSV(0, thissat, 255);
                    }
                    else
                    {
                        leds[i].r = 0;
                        leds[i].g = 0;
                        leds[i].b = 0;
                    }
                }
                LEDS.show();
            }
            if (curEffect == "white_temps")
            {
                // white_temps();
                int N9 = int(LED_COUNT / 9);
                for (int i = 0; i < LED_COUNT; i++)
                {
                    if (i >= 0 && i < N9)
                    {
                        leds[i].r = 255; //-CANDLE - 1900
                        leds[i].g = 147;
                        leds[i].b = 41;
                    }
                    if (i >= N9 && i < N9 * 2)
                    {
                        leds[i].r = 255; //-40W TUNG - 2600
                        leds[i].g = 197;
                        leds[i].b = 143;
                    }
                    if (i >= N9 * 2 && i < N9 * 3)
                    {
                        leds[i].r = 255; //-100W TUNG - 2850
                        leds[i].g = 214;
                        leds[i].b = 170;
                    }
                    if (i >= N9 * 3 && i < N9 * 4)
                    {
                        leds[i].r = 255; //-HALOGEN - 3200
                        leds[i].g = 241;
                        leds[i].b = 224;
                    }
                    if (i >= N9 * 4 && i < N9 * 5)
                    {
                        leds[i].r = 255; //-CARBON ARC - 5200
                        leds[i].g = 250;
                        leds[i].b = 244;
                    }
                    if (i >= N9 * 5 && i < N9 * 6)
                    {
                        leds[i].r = 255; //-HIGH NOON SUN - 5400
                        leds[i].g = 255;
                        leds[i].b = 251;
                    }
                    if (i >= N9 * 6 && i < N9 * 7)
                    {
                        leds[i].r = 255; //-DIRECT SUN - 6000
                        leds[i].g = 255;
                        leds[i].b = 255;
                    }
                    if (i >= N9 * 7 && i < N9 * 8)
                    {
                        leds[i].r = 201; //-OVERCAST SKY - 7000
                        leds[i].g = 226;
                        leds[i].b = 255;
                    }
                    if (i >= N9 * 8 && i < LED_COUNT)
                    {
                        leds[i].r = 64; //-CLEAR BLUE SKY - 20000
                        leds[i].g = 156;
                        leds[i].b = 255;
                    }
                }
                LEDS.show();
            }
            if (curEffect == "sin_bright_wave")
            {
                sin_bright_wave();
            }
            if (curEffect == "pop_horizontal")
            {
                // pop_horizontal();
                int ix;
                if (bouncedirection == 0)
                {
                    bouncedirection = 1;
                    ix = idex;
                }
                else if (bouncedirection == 1)
                {
                    bouncedirection = 0;
                    ix = horizontal_index(idex);
                    idex++;
                    if (idex > TOP_INDEX)
                    {
                        idex = 0;
                    }
                }
                for (int i = 0; i < LED_COUNT; i++)
                {
                    if (i == ix)
                    {
                        leds[i] = CHSV(thishue, thissat, 255);
                    }
                    else
                    {
                        leds[i].r = 0;
                        leds[i].g = 0;
                        leds[i].b = 0;
                    }
                }
                LEDS.show();
            }
            if (curEffect == "quad_bright_curve")
            {
                // quad_bright_curve();
                int ax;
                for (int x = 0; x < LED_COUNT; x++)
                {
                    if (x <= TOP_INDEX)
                    {
                        ax = x;
                    }
                    else if (x > TOP_INDEX)
                    {
                        ax = LED_COUNT - x;
                    }
                    int a = 1;
                    int b = 1;
                    int c = 0;
                    int iquad = -(ax * ax * a) + (ax * b) + c; //-ax2+bx+c
                    int hquad = -(TOP_INDEX * TOP_INDEX * a) + (TOP_INDEX * b) + c;
                    ibright = int((float(iquad) / float(hquad)) * 255);
                    leds[x] = CHSV(thishue, thissat, ibright);
                }
                LEDS.show();
            }
            if (curEffect == "flame")
            {
                flame();
            }
            if (curEffect == "rainbow_vertical")
            {
                // rainbow_vertical();
                idex++;
                if (idex > TOP_INDEX)
                {
                    idex = 0;
                }
                ihue = ihue + thisstep;
                if (ihue > 255)
                {
                    ihue = 0;
                }
                int idexA = idex;
                int idexB = horizontal_index(idexA);
                leds[idexA] = CHSV(ihue, thissat, 255);
                leds[idexB] = CHSV(ihue, thissat, 255);
                LEDS.show();
            }
            if (curEffect == "pacman")
            {
                // pacman();
                int s = int(LED_COUNT / 4);
                lcount++;
                if (lcount > 5)
                {
                    lcount = 0;
                }
                if (lcount == 0)
                {
                    for (int i = 0; i < LED_COUNT; i++)
                    {
                        set_color_led(i, 255, 255, 0);
                    }
                }
                if (lcount == 1 || lcount == 5)
                {
                    for (int i = 0; i < LED_COUNT; i++)
                    {
                        set_color_led(i, 255, 255, 0);
                    }
                    leds[s].r = 0;
                    leds[s].g = 0;
                    leds[s].b = 0;
                }
                if (lcount == 2 || lcount == 4)
                {
                    for (int i = 0; i < LED_COUNT; i++)
                    {
                        set_color_led(i, 255, 255, 0);
                    }
                    leds[s - 1].r = 0;
                    leds[s - 1].g = 0;
                    leds[s - 1].b = 0;
                    leds[s].r = 0;
                    leds[s].g = 0;
                    leds[s].b = 0;
                    leds[s + 1].r = 0;
                    leds[s + 1].g = 0;
                    leds[s + 1].b = 0;
                }
                if (lcount == 3)
                {
                    for (int i = 0; i < LED_COUNT; i++)
                    {
                        set_color_led(i, 255, 255, 0);
                    }
                    leds[s - 2].r = 0;
                    leds[s - 2].g = 0;
                    leds[s - 2].b = 0;
                    leds[s - 1].r = 0;
                    leds[s - 1].g = 0;
                    leds[s - 1].b = 0;
                    leds[s].r = 0;
                    leds[s].g = 0;
                    leds[s].b = 0;
                    leds[s + 1].r = 0;
                    leds[s + 1].g = 0;
                    leds[s + 1].b = 0;
                    leds[s + 2].r = 0;
                    leds[s + 2].g = 0;
                    leds[s + 2].b = 0;
                }
                LEDS.show();
            }
            if (curEffect == "random_color_pop")
            {
                // random_color_pop();
                idex = random(0, LED_COUNT);
                ihue = random(0, 255);
                one_color_all(0, 0, 0);
                leds[idex] = CHSV(ihue, thissat, 255);
                LEDS.show();
            }
            if (curEffect == "ems_lightsSTROBE")
            {
                ems_lightsSTROBE();
            }
            if (curEffect == "Strobe")
            {
                Strobe(R, G, B, 10, 50, _speed);
            }

            if (curEffect == "BouncingBalls")
            {
                BouncingBalls(R, G, B, 3);
            }
            if (curEffect == "BouncingColoredBalls")
            {
                byte colors[3][3] = {{0xff, 0, 0},
                                     {0xff, 0xff, 0xff},
                                     {0, 0, 0xff}};

                BouncingColoredBalls(3, colors);
            }

            if (curEffect == "rgb_propeller")
            {
                // rgb_propeller();
                idex++;
                int ghue = (thishue + 80) % 255;
                int bhue = (thishue + 160) % 255;
                int N3 = int(LED_COUNT / 3);
                int N6 = int(LED_COUNT / 6);
                int N12 = int(LED_COUNT / 12);
                for (int i = 0; i < N3; i++)
                {
                    int j0 = (idex + i + LED_COUNT - N12) % LED_COUNT;
                    int j1 = (j0 + N3) % LED_COUNT;
                    int j2 = (j1 + N3) % LED_COUNT;
                    leds[j0] = CHSV(thishue, thissat, 255);
                    leds[j1] = CHSV(ghue, thissat, 255);
                    leds[j2] = CHSV(bhue, thissat, 255);
                }
                LEDS.show();
            }
            if (curEffect == "kitt")
            {
                kitt();
            }
            if (curEffect == "matrix")
            {
                // matrix();
                int rand = random(0, 100);
                if (rand > 90)
                {
                    leds[0] = CHSV(thishue, thissat, 255);
                }
                else
                {
                    leds[0] = CHSV(thishue, thissat, 0);
                }
                copy_led_array();
                for (int i = 1; i < LED_COUNT; i++)
                {
                    leds[i].r = ledsX[i - 1][0];
                    leds[i].g = ledsX[i - 1][1];
                    leds[i].b = ledsX[i - 1][2];
                }
                LEDS.show();
            }
            if (curEffect == "new_rainbow_loop")
            {
                // new_rainbow_loop();
                ihue -= 1;
                fill_rainbow(leds, LED_COUNT, ihue);
                LEDS.show();
            }
            if (curEffect == "strip_march_ccw")
            {
                // strip_march_ccw();
                copy_led_array();
                int iCW;
                for (int i = 0; i < LED_COUNT; i++)
                {
                    iCW = adjacent_cw(i);
                    leds[i].r = ledsX[iCW][0];
                    leds[i].g = ledsX[iCW][1];
                    leds[i].b = ledsX[iCW][2];
                }
                LEDS.show();
            }
            if (curEffect == "strip_march_cw")
            {
                // strip_march_cw();
                copy_led_array();
                int iCCW;
                for (int i = 0; i < LED_COUNT; i++)
                {
                    iCCW = adjacent_ccw(i);
                    leds[i].r = ledsX[iCCW][0];
                    leds[i].g = ledsX[iCCW][1];
                    leds[i].b = ledsX[iCCW][2];
                }
                LEDS.show();
            }
            if (curEffect == "colorWipe")
            {
                colorWipe(R, G, B, thisdelay);
            }
            if (curEffect == "CylonBounce")
            {
                CylonBounce(R, G, B, 4, 10, thisdelay);
            }
            if (curEffect == "Fire")
            {
                // Fire(55, 120, thisdelay);
                static byte heat[LED_COUNT];
                int cooldown;
                int Cooling = 55, Sparking = 120;
                // Step 1.  Cool down every cell a little
                for (int i = 0; i < LED_COUNT; i++)
                {
                    cooldown = random(0, ((Cooling * 10) / LED_COUNT) + 2);

                    if (cooldown > heat[i])
                    {
                        heat[i] = 0;
                    }
                    else
                    {
                        heat[i] = heat[i] - cooldown;
                    }
                }

                // Step 2.  Heat from each cell drifts 'up' and diffuses a little
                for (int k = LED_COUNT - 1; k >= 2; k--)
                {
                    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
                }

                // Step 3.  Randomly ignite new 'sparks' near the bottom
                if (random(255) < Sparking)
                {
                    int y = random(7);
                    heat[y] = heat[y] + random(160, 255);
                    // heat[y] = random(160,255);
                }

                // Step 4.  Convert heat to LED colors
                for (int j = 0; j < LED_COUNT; j++)
                {
                    setPixelHeatColor(j, heat[j]);
                }

                FastLED.show();
            }
            if (curEffect == "NewKITT")
            {
                NewKITT(0xff, 0, 0, 8, 10, thisdelay);
            }
            if (curEffect == "rainbowCycle")
            {
                rainbowCycle(thisdelay);
            }
            if (curEffect == "TwinkleRandom")
            {
                TwinkleRandom(20, thisdelay, 1);
            }
            if (curEffect == "RunningLights")
            {
                RunningLights(0xff, 0xff, 0x00, thisdelay);
            }
            if (curEffect == "Sparkle")
            {
                Sparkle(random(255), random(255), random(60), _speed);
            }
            if (curEffect == "SnowSparkle")
            {
                SnowSparkle(0, 0, 0, 20, random(100, 1000));
            }
            if (curEffect == "theaterChase")
            {
                theaterChase(0xff, 0, 0, _speed);
            }
            if (curEffect == "theaterChaseRainbow")
            {
                theaterChaseRainbow(_speed);
            }

            if (curEffect == "demo_modeA")
            {
                demo_modeA();
            }
            if (curEffect == "demo_modeB")
            {
                demo_modeB();
            }
            if (curEffect == "meteorRain")
            {
                meteorRain(R, G, B, 10, 64, true, _speed);
            }

            // матрицы
            // *********** "дыхание" яркостью ***********
            if (curEffect == "brightnessRoutine")
            {
                brightnessRoutine();
            }
            // *********** снегопад 2.0 ***********
            if (curEffect == "snowRoutine")
            {
                snowRoutine();
            }
            // ***************************** БЛУДНЫЙ КУБИК *****************************
            if (curEffect == "ballRoutine")
            {
                ballRoutine();
            }
            // *********** радуга заливка ***********
            if (curEffect == "rainbowRoutine")
            {
                rainbowRoutine();
            }
            // *********** радуга дигональная ***********
            if (curEffect == "rainbowDiagonalRoutine")
            {
                rainbowDiagonalRoutine();
            }
            // *********** радуга активных светодиодов (рисунка) ***********
            if (curEffect == "rainbowColorsRoutine")
            {
                rainbowColorsRoutine();
            }
            // ********************** огонь **********************
            if (curEffect == "fireRoutine")
            {
                fireRoutine();
            }
            // **************** МАТРИЦА *****************
            if (curEffect == "matrixRoutine")
            {
                matrixRoutine();
            }
            // ********************* ЗВЕЗДОПАД ******************
            if (curEffect == "starfallRoutine")
            {
                starfallRoutine();
            }
            // рандомные гаснущие вспышки
            if (curEffect == "sparklesRoutine")
            {
                sparklesRoutine();
            }
            // ----------------------------- СВЕТЛЯКИ ------------------------------
            if (curEffect == "lightersRoutine")
            {
                lightersRoutine();
            }
            // ------------- ПЕЙНТБОЛ -------------
            if (curEffect == "lightBallsRoutine")
            {
                lightBallsRoutine();
            }
            // ------------- ВОДОВОРОТ -------------
            if (curEffect == "swirlRoutine")
            {
                swirlRoutine();
            }
            // ------------- крутящаяся радуга матрица -------------
            if (curEffect == "rainbow_loop_matrix")
            {
                idex++;
                ihue = ihue + thisstep;
                if (idex >= LED_COUNT)
                {
                    idex = 0;
                }
                if (ihue > 255)
                {
                    ihue = 0;
                }

                for (byte i = 0; i < WIDTH; i++)
                {
                    CHSV thisColor = CHSV(ihue, thissat, 255);
                    for (byte j = 0; j < HEIGHT; j++)
                        drawPixelXY(i, j, thisColor); // leds[getPixelNumber(i, j)] = thisColor;
                }
            }

            FastLED.show();
        }
    }

    IoTValue
    execute(String command, std::vector<IoTValue> &param)
    {

        if (command == "effect")
        {
            curEffect = "none";

            /////////////////////////////////////
            if (param[0].valS == "rainbow_fade")
            {
                curEffect = "rainbow_fade";
            }
            if (param[0].valS == "rainbow_loop")
            {
                curEffect = "rainbow_loop";
            }
            if (param[0].valS == "random_burst")
            {
                curEffect = "random_burst";
            }
            if (param[0].valS == "color_bounce")
            {
                curEffect = "color_bounce";
            }
            if (param[0].valS == "color_bounceFADE")
            {
                curEffect = "color_bounceFADE";
            }
            if (param[0].valS == "ems_lightsONE")
            {
                curEffect = "ems_lightsONE";
            }
            if (param[0].valS == "ems_lightsALL")
            {
                curEffect = "ems_lightsALL";
            }
            if (param[0].valS == "pulse_one_color_all")
            {
                curEffect = "pulse_one_color_all";
            }
            if (param[0].valS == "pulse_one_color_all_rev")
            {
                curEffect = "pulse_one_color_all_rev";
            }
            if (param[0].valS == "fade_vertical")
            {
                curEffect = "fade_vertical";
            }
            if (param[0].valS == "rule30")
            {
                curEffect = "rule30";
            }
            if (param[0].valS == "random_march")
            {
                curEffect = "random_march";
            }
            if (param[0].valS == "rwb_march")
            {
                curEffect = "rwb_march";
            }
            if (param[0].valS == "radiation")
            {
                curEffect = "radiation";
            }
            if (param[0].valS == "color_loop_vardelay")
            {
                curEffect = "color_loop_vardelay";
            }
            if (param[0].valS == "white_temps")
            {
                curEffect = "white_temps";
            }
            if (param[0].valS == "sin_bright_wave")
            {
                curEffect = "sin_bright_wave";
            }
            if (param[0].valS == "pop_horizontal")
            {
                curEffect = "pop_horizontal";
            }
            if (param[0].valS == "quad_bright_curve")
            {
                curEffect = "quad_bright_curve";
            }
            if (param[0].valS == "flame")
            {
                curEffect = "flame";
            }
            if (param[0].valS == "rainbow_vertical")
            {
                curEffect = "rainbow_vertical";
            }
            if (param[0].valS == "pacman")
            {
                curEffect = "pacman";
            }
            if (param[0].valS == "random_color_pop")
            {
                curEffect = "random_color_pop";
            }
            if (param[0].valS == "ems_lightsSTROBE")
            {
                curEffect = "ems_lightsSTROBE";
            }
            if (param[0].valS == "rgb_propeller")
            {
                curEffect = "rgb_propeller";
            }
            if (param[0].valS == "kitt")
            {
                curEffect = "kitt";
            }
            if (param[0].valS == "matrix")
            {
                curEffect = "matrix";
            }
            if (param[0].valS == "new_rainbow_loop")
            {
                curEffect = "new_rainbow_loop";
            }
            if (param[0].valS == "strip_march_ccw")
            {
                curEffect = "strip_march_ccw";
            }
            if (param[0].valS == "strip_march_cw")
            {
                curEffect = "strip_march_cw";
            }
            if (param[0].valS == "colorWipe")
            {
                curEffect = "colorWipe";
            }
            if (param[0].valS == "CylonBounce")
            {
                curEffect = "CylonBounce";
            }
            if (param[0].valS == "Fire")
            {
                curEffect = "Fire";
            }
            if (param[0].valS == "NewKITT")
            {
                curEffect = "NewKITT";
            }
            if (param[0].valS == "rainbowCycle")
            {
                curEffect = "rainbowCycle";
            }
            if (param[0].valS == "TwinkleRandom")
            {
                curEffect = "TwinkleRandom";
            }
            if (param[0].valS == "RunningLights")
            {
                curEffect = "RunningLights";
            }
            if (param[0].valS == "Sparkle")
            {
                curEffect = "Sparkle";
            }
            if (param[0].valS == "SnowSparkle")
            {
                curEffect = "SnowSparkle";
            }
            if (param[0].valS == "theaterChase")
            {
                curEffect = "theaterChase";
            }
            if (param[0].valS == "theaterChaseRainbow")
            {
                curEffect = "theaterChaseRainbow";
            }
            if (param[0].valS == "Strobe")
            {
                curEffect = "Strobe";
            }
            if (param[0].valS == "BouncingBalls")
            {
                curEffect = "BouncingBalls";
            }
            if (param[0].valS == "BouncingColoredBalls")
            {
                curEffect = "BouncingColoredBalls";
            }
            if (param[0].valS == "demo_modeA")
            {
                curEffect = "demo_modeA";
            }
            if (param[0].valS == "demo_modeB")
            {
                curEffect = "demo_modeB";
            }
            if (param[0].valS == "meteorRain")
            {
                curEffect = "meteorRain";
            }

            // матрицы

            // *********** "дыхание" яркостью ***********
            if (param[0].valS == "brightnessRoutine")
            {
                curEffect = "brightnessRoutine";
            }
            // *********** снегопад 2.0 ***********
            if (param[0].valS == "snowRoutine")
            {
                curEffect = "snowRoutine";
            }
            // ***************************** БЛУДНЫЙ КУБИК *****************************
            if (param[0].valS == "ballRoutine")
            {
                curEffect = "ballRoutine";
            }
            // *********** радуга заливка ***********
            if (param[0].valS == "rainbowRoutine")
            {
                curEffect = "rainbowRoutine";
            }
            // *********** радуга дигональная ***********
            if (param[0].valS == "rainbowDiagonalRoutine")
            {
                curEffect = "rainbowDiagonalRoutine";
            }
            // *********** радуга активных светодиодов (рисунка) ***********
            if (param[0].valS == "rainbowColorsRoutine")
            {
                curEffect = "rainbowColorsRoutine";
            }
            // ********************** огонь **********************
            if (param[0].valS == "fireRoutine")
            {
                curEffect = "fireRoutine";
            }
            // **************** МАТРИЦА *****************
            if (param[0].valS == "matrixRoutine")
            {
                curEffect = "matrixRoutine";
            }
            // ********************* ЗВЕЗДОПАД ******************
            if (param[0].valS == "starfallRoutine")
            {
                curEffect = "starfallRoutine";
            }
            // рандомные гаснущие вспышки
            if (param[0].valS == "sparklesRoutine")
            {
                curEffect = "sparklesRoutine";
            }
            // ----------------------------- СВЕТЛЯКИ ------------------------------
            if (param[0].valS == "lightersRoutine")
            {
                curEffect = "lightersRoutine";
            }
            // ------------- ПЕЙНТБОЛ -------------
            if (param[0].valS == "lightBallsRoutine")
            {
                curEffect = "lightBallsRoutine";
            }
            // ------------- ВОДОВОРОТ -------------
            if (param[0].valS == "swirlRoutine")
            {
                curEffect = "swirlRoutine";
            }
            // ------------- крутящаяся радуга матрица -------------
            if (param[0].valS == "rainbow_loop_matrix")
            {
                curEffect = "rainbow_loop_matrix";
            }

            ///////////////////////////////////////////////////
        }
        else if (command == "brightness")
        {
            _brightness = param[0].valD;
            LEDS.setBrightness(_brightness);
            LEDS.show();
        }
        else if (command == "speed")
        {
            _speed = param[0].valD;
        }
        else if (command == "dalay")
        {
            _dalay = param[0].valD;
        }
        else if (command == "color")
        {
            R = param[0].valD;
            G = param[1].valD;
            B = param[2].valD;
            setAll(R, G, B);
            LEDS.show();
        }
        else if (command == "Off")
        {
            curEffect = "none";
            setAll(0, 0, 0);
            LEDS.show();
        }
        else if (command == "On")
        {
            curEffect = "none";

            if (param.size() == 3)
            {
                R = param[0].valD;
                G = param[1].valD;
                B = param[2].valD;
                setAll(R, G, B);
            }
            else
            {
                setAll(R, G, B);
            }
            LEDS.show();
        }

        return {};
    }
    void setValue(const IoTValue &Value, bool genEvent = true)
    {
        value = Value;
        _brightness = map(value.valD, 1, 1024, 1, 100);
        LEDS.setBrightness(_brightness);
        regEvent(value.valD, "GyverLAMP", false, genEvent);
        // setAll(R, G, B);
        LEDS.show();
    }
    ~GyverLAMP(){};
};

void *getAPI_GyverLAMP(String subtype, String param)
{
    if (subtype == F("GyverLAMP"))
    {
        return new GyverLAMP(param);
    }
    else
    {
        return nullptr;
    }
}
