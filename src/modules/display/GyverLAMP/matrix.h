#include "utility.h"

byte hue;
boolean loadingFlag = true;
// **************** НАСТРОЙКИ ЭФФЕКТОВ ****************
// эффект "синусоиды" - ОТКЛЮЧЕН
#define WAVES_AMOUNT 2 // количество синусоид

// эффект "шарики"
#define BALLS_AMOUNT 3 // количество "шариков"
#define CLEAR_PATH 1   // очищать путь
#define BALL_TRACK 1   // (0 / 1) - вкл/выкл следы шариков
#define DRAW_WALLS 0   // режим с рисованием препятствий для шаров (не работает на ESP и STM32)
#define TRACK_STEP 70  // длина хвоста шарика (чем больше цифра, тем хвост короче)

// эффект "квадратик"
#define BALL_SIZE 3    // размер шара
#define RANDOM_COLOR 1 // случайный цвет при отскоке

// эффект "огонь"
#define SPARKLES 1 // вылетающие угольки вкл выкл
#define HUE_ADD 0  // добавка цвета в огонь (от 0 до 230) - меняет весь цвет пламени

// эффект "кометы"
#define TAIL_STEP 100  // длина хвоста кометы
#define SATURATION 150 // насыщенность кометы (от 0 до 255)
#define STAR_DENSE 60  // количество (шанс появления) комет

// эффект "конфетти"
#define DENSE 3        // плотность конфетти
#define BRIGHT_STEP 70 // шаг уменьшения яркости

// эффект "снег"
#define SNOW_DENSE 10 // плотность снегопада

// эффект "Светляки"
#define LIGHTERS_AM 35 // количество светляков

uint32_t globalColor = 0xffffff; // Цвет рисования при запуске белый
unsigned char matrixValue[8][16];
unsigned char line[WIDTH];
int pcnt = 0;
int effectSpeed = _speed; // скрость изменения эффекта
uint8_t USE_SEGMENTS = 1;
uint8_t BorderWidth = 0;
uint8_t dir_mx, seg_num, seg_size, seg_offset;
uint16_t XY(uint8_t, uint8_t); // __attribute__ ((weak));

// эффекты матрицы

// *********** "дыхание" яркостью ***********
boolean brightnessDirection;
byte breathBrightness; // Яркость эффекта "Дыхание"
byte globalBrightness = _brightness;
void brightnessRoutine()
{
    if (brightnessDirection)
    {
        breathBrightness += 2;
        if (breathBrightness > globalBrightness - 1)
        {
            brightnessDirection = false;
        }
    }
    else
    {
        breathBrightness -= 2;
        if (breathBrightness < 1)
        {
            brightnessDirection = true;
        }
    }
    FastLED.setBrightness(breathBrightness);
}

// *********** снегопад 2.0 ***********
void snowRoutine()
{

    // сдвигаем всё вниз
    for (byte x = 0; x < WIDTH; x++)
    {
        for (byte y = 0; y < HEIGHT - 1; y++)
        {
            drawPixelXY(x, y, getPixColorXY(x, y + 1));
        }
    }

    for (byte x = 0; x < WIDTH; x++)
    {
        // заполняем случайно верхнюю строку
        // а также не даём двум блокам по вертикали вместе быть
        if (getPixColorXY(x, HEIGHT - 2) == 0 && (random(0, SNOW_DENSE) == 0))
            drawPixelXY(x, HEIGHT - 1, 0xE0FFFF - 0x101010 * random(0, 4));
        else
            drawPixelXY(x, HEIGHT - 1, 0x000000);
    }
}

// ***************************** БЛУДНЫЙ КУБИК *****************************
int coordB[2];
int8_t vectorB[2];
CRGB ballColor;

void ballRoutine()
{
    if (loadingFlag)
    {
        for (byte i = 0; i < 2; i++)
        {
            coordB[i] = WIDTH / 2 * 10;
            vectorB[i] = random(8, 20);
            ballColor = CHSV(random(0, 9) * 28, 255, 255);
        }

        loadingFlag = false;
    }
    for (byte i = 0; i < 2; i++)
    {
        coordB[i] += vectorB[i];
        if (coordB[i] < 0)
        {
            coordB[i] = 0;
            vectorB[i] = -vectorB[i];
            if (RANDOM_COLOR)
                ballColor = CHSV(random(0, 9) * 28, 255, 255);
            // vectorB[i] += random(0, 6) - 3;
        }
    }
    if (coordB[0] > (WIDTH - BALL_SIZE) * 10)
    {
        coordB[0] = (WIDTH - BALL_SIZE) * 10;
        vectorB[0] = -vectorB[0];
        if (RANDOM_COLOR)
            ballColor = CHSV(random(0, 9) * 28, 255, 255);
        // vectorB[0] += random(0, 6) - 3;
    }
    if (coordB[1] > (HEIGHT - BALL_SIZE) * 10)
    {
        coordB[1] = (HEIGHT - BALL_SIZE) * 10;
        vectorB[1] = -vectorB[1];
        if (RANDOM_COLOR)
            ballColor = CHSV(random(0, 9) * 28, 255, 255);
        // vectorB[1] += random(0, 6) - 3;
    }
    FastLED.clear();
    for (byte i = 0; i < BALL_SIZE; i++)
        for (byte j = 0; j < BALL_SIZE; j++)
            leds[getPixelNumber(coordB[0] / 10 + i, coordB[1] / 10 + j)] = ballColor;
}

// *********** радуга заливка ***********
void rainbowRoutine()
{

    hue += 3;
    for (byte i = 0; i < WIDTH; i++)
    {
        CHSV thisColor = CHSV((byte)(hue + i * float(255 / WIDTH)), 255, 255);
        for (byte j = 0; j < HEIGHT; j++)
            drawPixelXY(i, j, thisColor); // leds[getPixelNumber(i, j)] = thisColor;
    }
}

// *********** радуга дигональная ***********
void rainbowDiagonalRoutine()
{

    hue += 3;
    for (byte x = 0; x < WIDTH; x++)
    {
        for (byte y = 0; y < HEIGHT; y++)
        {
            CHSV thisColor = CHSV((byte)(hue + (float)(WIDTH / HEIGHT * x + y) * (float)(255 / 100)), 255, 255);
            drawPixelXY(x, y, thisColor); // leds[getPixelNumber(i, j)] = thisColor;
        }
    }
}

// *********** радуга активных светодиодов (рисунка) ***********
void rainbowColorsRoutine()
{
    hue++;
    for (byte i = 0; i < WIDTH; i++)
    {
        CHSV thisColor = CHSV((byte)(hue + i * float(255 / WIDTH)), 255, 255);
        for (byte j = 0; j < HEIGHT; j++)
            if (getPixColor(getPixelNumber(i, j)) > 0)
                drawPixelXY(i, j, thisColor);
    }
}

// ****************************** ОГОНЬ ******************************
// ********************** огонь **********************
// these values are substracetd from the generated values to give a shape to the animation
const unsigned char valueMask[8][16] PROGMEM = {
    {32, 0, 0, 0, 0, 0, 0, 32, 32, 0, 0, 0, 0, 0, 0, 32},
    {64, 0, 0, 0, 0, 0, 0, 64, 64, 0, 0, 0, 0, 0, 0, 64},
    {96, 32, 0, 0, 0, 0, 32, 96, 96, 32, 0, 0, 0, 0, 32, 96},
    {128, 64, 32, 0, 0, 32, 64, 128, 128, 64, 32, 0, 0, 32, 64, 128},
    {160, 96, 64, 32, 32, 64, 96, 160, 160, 96, 64, 32, 32, 64, 96, 160},
    {192, 128, 96, 64, 64, 96, 128, 192, 192, 128, 96, 64, 64, 96, 128, 192},
    {255, 160, 128, 96, 96, 128, 160, 255, 255, 160, 128, 96, 96, 128, 160, 255},
    {255, 192, 160, 128, 128, 160, 192, 255, 255, 192, 160, 128, 128, 160, 192, 255}};

// these are the hues for the fire,
// should be between 0 (red) to about 25 (yellow)
const unsigned char hueMask[8][16] PROGMEM = {
    {1, 11, 19, 25, 25, 22, 11, 1, 1, 11, 19, 25, 25, 22, 11, 1},
    {1, 8, 13, 19, 25, 19, 8, 1, 1, 8, 13, 19, 25, 19, 8, 1},
    {1, 8, 13, 16, 19, 16, 8, 1, 1, 8, 13, 16, 19, 16, 8, 1},
    {1, 5, 11, 13, 13, 13, 5, 1, 1, 5, 11, 13, 13, 13, 5, 1},
    {1, 5, 11, 11, 11, 11, 5, 1, 1, 5, 11, 11, 11, 11, 5, 1},
    {0, 1, 5, 8, 8, 5, 1, 0, 0, 1, 5, 8, 8, 5, 1, 0},
    {0, 0, 1, 5, 5, 1, 0, 0, 0, 0, 1, 5, 5, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0}};
// Randomly generate the next line (matrix row)

void generateLine()
{
    for (uint8_t x = 0; x < WIDTH; x++)
    {
        line[x] = random(64, 255);
    }
}

// shift all values in the matrix up one row

void shiftUp()
{
    for (uint8_t y = HEIGHT - 1; y > 0; y--)
    {
        for (uint8_t x = 0; x < WIDTH; x++)
        {
            uint8_t newX = x;
            if (x > 15)
                newX = x % 16;
            if (y > 7)
                continue;
            matrixValue[y][newX] = matrixValue[y - 1][newX];
        }
    }

    for (uint8_t x = 0; x < WIDTH; x++)
    {
        uint8_t newX = x;
        if (x > 15)
            newX = x % 16;
        matrixValue[0][newX] = line[newX];
    }
}

// draw a frame, interpolating between 2 "key frames"
// @param pcnt percentage of interpolation

void drawFrame(int pcnt)
{
    int nextv;

    // each row interpolates with the one before it
    for (unsigned char y = HEIGHT - 1; y > 0; y--)
    {
        for (unsigned char x = 0; x < WIDTH; x++)
        {
            uint8_t newX = x;
            if (x > 15)
                newX = x % 16;
            if (y < 8)
            {
                nextv =
                    (((100.0 - pcnt) * matrixValue[y][newX] + pcnt * matrixValue[y - 1][newX]) / 100.0) - pgm_read_byte(&(valueMask[y][newX]));

                CRGB color = CHSV(
                    HUE_ADD + pgm_read_byte(&(hueMask[y][newX])), // H
                    255,                                          // S
                    (uint8_t)max(0, nextv)                        // V
                );

                leds[getPixelNumber(x, y)] = color;
            }
            else if (y == 8 && SPARKLES)
            {
                if (random(0, 20) == 0 && getPixColorXY(x, y - 1) != 0)
                    drawPixelXY(x, y, getPixColorXY(x, y - 1));
                else
                    drawPixelXY(x, y, 0);
            }
            else if (SPARKLES)
            {

                // старая версия для яркости
                if (getPixColorXY(x, y - 1) > 0)
                    drawPixelXY(x, y, getPixColorXY(x, y - 1));
                else
                    drawPixelXY(x, y, 0);
            }
        }
    }

    // first row interpolates with the "next" line
    for (unsigned char x = 0; x < WIDTH; x++)
    {
        uint8_t newX = x;
        if (x > 15)
            newX = x % 16;
        CRGB color = CHSV(
            HUE_ADD + pgm_read_byte(&(hueMask[0][newX])),                                  // H
            255,                                                                           // S
            (uint8_t)(((100.0 - pcnt) * matrixValue[0][newX] + pcnt * line[newX]) / 100.0) // V
        );
        // leds[getPixelNumber(newX, 0)] = color; // На форуме пишут что это ошибка - вместо newX должно быть x, иначе
        leds[getPixelNumber(x, 0)] = color; // на матрицах шире 16 столбцов нижний правый угол неработает
    }
}
void fireRoutine()
{
    if (loadingFlag)
    {

        loadingFlag = false;
        FastLED.clear();
        generateLine();
        memset(matrixValue, 0, sizeof(matrixValue));
    }
    if (pcnt >= 100)
    {
        shiftUp();
        generateLine();
        pcnt = 0;
    }
    drawFrame(pcnt);
    pcnt += 30;
}

// **************** МАТРИЦА *****************
void matrixRoutine()
{
    if (loadingFlag)
    {
        loadingFlag = false;

        FastLED.clear();
    }
    for (byte x = 0; x < WIDTH; x++)
    {
        // заполняем случайно верхнюю строку
        uint32_t thisColor = getPixColorXY(x, HEIGHT - 1);
        if (thisColor == 0)
            drawPixelXY(x, HEIGHT - 1, 0x00FF00 * (random(0, 10) == 0));
        else if (thisColor < 0x002000)
            drawPixelXY(x, HEIGHT - 1, 0);
        else
            drawPixelXY(x, HEIGHT - 1, thisColor - 0x002000);
    }

    // сдвигаем всё вниз
    for (byte x = 0; x < WIDTH; x++)
    {
        for (byte y = 0; y < HEIGHT - 1; y++)
        {
            drawPixelXY(x, y, getPixColorXY(x, y + 1));
        }
    }
}

// ********************* ЗВЕЗДОПАД ******************
void fadePixel(byte i, byte j, byte step)
{ // новый фейдер
    int pixelNum = getPixelNumber(i, j);
    if (getPixColor(pixelNum) == 0)
        return;

    if (leds[pixelNum].r >= 30 ||
        leds[pixelNum].g >= 30 ||
        leds[pixelNum].b >= 30)
    {
        leds[pixelNum].fadeToBlackBy(step);
    }
    else
    {
        leds[pixelNum] = 0;
    }
}
// функция плавного угасания цвета для всех пикселей
void fader(byte step)
{
    for (byte i = 0; i < WIDTH; i++)
    {
        for (byte j = 0; j < HEIGHT; j++)
        {
            fadePixel(i, j, step);
        }
    }
}

void starfallRoutine()
{

    // заполняем головами комет левую и верхнюю линию
    for (byte i = HEIGHT / 2; i < HEIGHT; i++)
    {
        if (getPixColorXY(0, i) == 0 && (random(0, STAR_DENSE) == 0) && getPixColorXY(0, i + 1) == 0 && getPixColorXY(0, i - 1) == 0)
            leds[getPixelNumber(0, i)] = CHSV(random(0, 200), SATURATION, 255);
    }
    for (byte i = 0; i < WIDTH / 2; i++)
    {
        if (getPixColorXY(i, HEIGHT - 1) == 0 && (random(0, STAR_DENSE) == 0) && getPixColorXY(i + 1, HEIGHT - 1) == 0 && getPixColorXY(i - 1, HEIGHT - 1) == 0)
            leds[getPixelNumber(i, HEIGHT - 1)] = CHSV(random(0, 200), SATURATION, 255);
    }

    // сдвигаем по диагонали
    for (byte y = 0; y < HEIGHT - 1; y++)
    {
        for (byte x = WIDTH - 1; x > 0; x--)
        {
            drawPixelXY(x, y, getPixColorXY(x - 1, y + 1));
        }
    }

    // уменьшаем яркость левой и верхней линии, формируем "хвосты"
    for (byte i = HEIGHT / 2; i < HEIGHT; i++)
    {
        fadePixel(0, i, TAIL_STEP);
    }
    for (byte i = 0; i < WIDTH / 2; i++)
    {
        fadePixel(i, HEIGHT - 1, TAIL_STEP);
    }
}

// рандомные гаснущие вспышки
void sparklesRoutine()
{

    for (byte i = 0; i < DENSE; i++)
    {
        byte x = random(0, WIDTH);
        byte y = random(0, HEIGHT);
        if (getPixColorXY(x, y) == 0)
            leds[getPixelNumber(x, y)] = CHSV(random(0, 255), 255, 255);
    }
    fader(BRIGHT_STEP);
}

// ----------------------------- СВЕТЛЯКИ ------------------------------
int lightersPos[2][LIGHTERS_AM];
int8_t lightersSpeed[2][LIGHTERS_AM];
CHSV lightersColor[LIGHTERS_AM];
byte loopCounter;

int angle[LIGHTERS_AM];
int speedV[LIGHTERS_AM];
int8_t angleSpeed[LIGHTERS_AM];

void lightersRoutine()
{
    if (loadingFlag)
    {
        loadingFlag = false;
        randomSeed(millis());
        for (byte i = 0; i < LIGHTERS_AM; i++)
        {
            lightersPos[0][i] = random(0, WIDTH * 10);
            lightersPos[1][i] = random(0, HEIGHT * 10);
            lightersSpeed[0][i] = random(-10, 10);
            lightersSpeed[1][i] = random(-10, 10);
            lightersColor[i] = CHSV(random(0, 255), 255, 255);
        }
    }
    FastLED.clear();
    if (++loopCounter > 20)
        loopCounter = 0;
    for (byte i = 0; i < map(LIGHTERS_AM, 0, 255, 5, 150); i++)
    {
        if (loopCounter == 0)
        { // меняем скорость каждые 255 отрисовок
            lightersSpeed[0][i] += random(-3, 4);
            lightersSpeed[1][i] += random(-3, 4);
            lightersSpeed[0][i] = constrain(lightersSpeed[0][i], -20, 20);
            lightersSpeed[1][i] = constrain(lightersSpeed[1][i], -20, 20);
        }

        lightersPos[0][i] += lightersSpeed[0][i];
        lightersPos[1][i] += lightersSpeed[1][i];

        if (lightersPos[0][i] < 0)
            lightersPos[0][i] = (WIDTH - 1) * 10;
        if (lightersPos[0][i] >= WIDTH * 10)
            lightersPos[0][i] = 0;

        if (lightersPos[1][i] < 0)
        {
            lightersPos[1][i] = 0;
            lightersSpeed[1][i] = -lightersSpeed[1][i];
        }
        if (lightersPos[1][i] >= (HEIGHT - 1) * 10)
        {
            lightersPos[1][i] = (HEIGHT - 1) * 10;
            lightersSpeed[1][i] = -lightersSpeed[1][i];
        }
        drawPixelXY(lightersPos[0][i] / 10, lightersPos[1][i] / 10, lightersColor[i]);
    }
}

// ------------- ПЕЙНТБОЛ -------------

void lightBallsRoutine()
{
    if (loadingFlag)
    {
        loadingFlag = false;

        FastLED.clear();                                                                    // очистить
        dir_mx = WIDTH > HEIGHT ? 0 : 1;                                                    // 0 - квадратные сегменты расположены горизонтально, 1 - вертикально
        seg_num = dir_mx == 0 ? (WIDTH / HEIGHT) : (HEIGHT / WIDTH);                        // вычисляем количество сегментов, умещающихся на матрице
        seg_size = dir_mx == 0 ? HEIGHT : WIDTH;                                            // Размер квадратного сегмента (высота и ширина равны)
        seg_offset = ((dir_mx == 0 ? WIDTH : HEIGHT) - seg_size * seg_num) / (seg_num + 1); // смещение от края матрицы и между сегментами
        BorderWidth = 0;
    }

    // Apply some blurring to whatever's already on the matrix
    // Note that we never actually clear the matrix, we just constantly
    // blur it repeatedly.  Since the blurring is 'lossy', there's
    // an automatic trend toward black -- by design.
    uint8_t blurAmount = dim8_raw(beatsin8(2, 64, 100));
    blur2d(leds, WIDTH, HEIGHT, blurAmount);

    // The color of each point shifts over time, each at a different speed.
    uint32_t ms = millis();
    int16_t idx;

    byte cnt = map(effectSpeed, 0, 255, 1, 4);

    if (USE_SEGMENTS != 0)
    {
        // Для неквадратных - вычленяем квадратные сегменты, которые равномерно распределяем по ширине / высоте матрицы
        uint8_t i = beatsin8(91, 0, seg_size - BorderWidth - 1);
        uint8_t j = beatsin8(109, 0, seg_size - BorderWidth - 1);
        uint8_t k = beatsin8(73, 0, seg_size - BorderWidth - 1);
        uint8_t m = beatsin8(123, 0, seg_size - BorderWidth - 1);

        uint8_t d1 = ms / 29;
        uint8_t d2 = ms / 41;
        uint8_t d3 = ms / 73;
        uint8_t d4 = ms / 97;

        for (uint8_t ii = 0; ii < seg_num; ii++)
        {
            delay(0); // Для предотвращения ESP8266 Watchdog Timer
            uint8_t cx = dir_mx == 0 ? (seg_offset * (ii + 1) + seg_size * ii) : 0;
            uint8_t cy = dir_mx == 0 ? 0 : (seg_offset * (ii + 1) + seg_size * ii);
            uint8_t color_shift = ii * 50;
            if (cnt <= 1)
            {
                idx = XY(i + cx, j + cy);
                leds[idx] += CHSV(color_shift + d1, 200U, 255U);
            }
            if (cnt <= 2)
            {
                idx = XY(j + cx, k + cy);
                leds[idx] += CHSV(color_shift + d2, 200U, 255U);
            }
            if (cnt <= 3)
            {
                idx = XY(k + cx, m + cy);
                leds[idx] += CHSV(color_shift + d3, 200U, 255U);
            }
            if (cnt <= 4)
            {
                idx = XY(m + cx, i + cy);
                leds[idx] += CHSV(color_shift + d4, 200U, 255U);
            }

            // При соединении матрицы из угла вверх или вниз почему-то слева и справа узора остаются полосы, которые
            // не гаснут обычным blur - гасим полоски левой и правой стороны дополнительно.
            // При соединении из угла влево или вправо или на неквадратных матрицах такого эффекта не наблюдается
            for (byte i2 = cy; i2 < cy + seg_size; i2++)
            {
                fadePixel(cx + BorderWidth, i2, 15);
                fadePixel(cx + seg_size - BorderWidth - 1, i2, 15);
            }
        }
    }
    else
    {
        uint8_t i = beatsin8(91, BorderWidth, WIDTH - BorderWidth - 1);
        uint8_t j = beatsin8(109, BorderWidth, HEIGHT - BorderWidth - 1);
        uint8_t k = beatsin8(73, BorderWidth, WIDTH - BorderWidth - 1);
        uint8_t m = beatsin8(123, BorderWidth, HEIGHT - BorderWidth - 1);

        if (cnt <= 1)
        {
            idx = XY(i, j);
            leds[idx] += CHSV(ms / 29, 200U, 255U);
        }
        if (cnt <= 2)
        {
            idx = XY(k, j);
            leds[idx] += CHSV(ms / 41, 200U, 255U);
        }
        if (cnt <= 3)
        {
            idx = XY(k, m);
            leds[idx] += CHSV(ms / 73, 200U, 255U);
        }
        if (cnt <= 4)
        {
            idx = XY(i, m);
            leds[idx] += CHSV(ms / 97, 200U, 255U);
        }

        if (WIDTH == HEIGHT)
        {
            // При соединении матрицы из угла вверх или вниз почему-то слева и справа узора остаются полосы, которые
            // не гаснут обычным blur - гасим полоски левой и правой стороны дополнительно.
            // При соединении из угла влево или вправо или на неквадратных матрицах такого эффекта не наблюдается
            for (byte i = 0; i < HEIGHT; i++)
            {
                fadePixel(0, i, 15);
                fadePixel(WIDTH - 1, i, 15);
            }
        }
    }
}
// ------------- ВОДОВОРОТ -------------

void swirlRoutine()
{
    if (loadingFlag)
    {
        loadingFlag = false;

        FastLED.clear();                                                                    // очистить
        dir_mx = WIDTH > HEIGHT ? 0 : 1;                                                    // 0 - квадратные сегменты расположены горизонтально, 1 - вертикально
        seg_num = dir_mx == 0 ? (WIDTH / HEIGHT) : (HEIGHT / WIDTH);                        // вычисляем количество сегментов, умещающихся на матрице
        seg_size = dir_mx == 0 ? HEIGHT : WIDTH;                                            // Размер квадратного сегмента (высота и ширина равны)
        seg_offset = ((dir_mx == 0 ? WIDTH : HEIGHT) - seg_size * seg_num) / (seg_num + 1); // смещение от края матрицы и между сегментами
        BorderWidth = seg_num == 1 ? 0 : 1;
    }

    // Apply some blurring to whatever's already on the matrix
    // Note that we never actually clear the matrix, we just constantly
    // blur it repeatedly.  Since the blurring is 'lossy', there's
    // an automatic trend toward black -- by design.
    uint8_t blurAmount = dim8_raw(beatsin8(2, 64, 100));
    blur2d(leds, WIDTH, HEIGHT, blurAmount);

    uint32_t ms = millis();
    int16_t idx;

    if (USE_SEGMENTS != 0)
    {
        // Use two out-of-sync sine waves
        uint8_t i = beatsin8(41, 0, seg_size - BorderWidth - 1);
        uint8_t j = beatsin8(27, 0, seg_size - BorderWidth - 1);

        // Also calculate some reflections
        uint8_t ni = (seg_size - 1) - i;
        uint8_t nj = (seg_size - 1) - j;

        uint8_t d1 = ms / 11;
        uint8_t d2 = ms / 13;
        uint8_t d3 = ms / 17;
        uint8_t d4 = ms / 29;
        uint8_t d5 = ms / 37;
        uint8_t d6 = ms / 41;

        for (uint8_t ii = 0; ii < seg_num; ii++)
        {
            delay(0); // Для предотвращения ESP8266 Watchdog Timer
            uint8_t cx = dir_mx == 0 ? (seg_offset * (ii + 1) + seg_size * ii) : 0;
            uint8_t cy = dir_mx == 0 ? 0 : (seg_offset * (ii + 1) + seg_size * ii);
            uint8_t color_shift = ii * 50;

            // The color of each point shifts over time, each at a different speed.
            idx = XY(i + cx, j + cy);
            leds[idx] += CHSV(color_shift + d1, 200, 192);
            idx = XY(ni + cx, nj + cy);
            leds[idx] += CHSV(color_shift + d2, 200, 192);
            idx = XY(i + cx, nj + cy);
            leds[idx] += CHSV(color_shift + d3, 200, 192);
            idx = XY(ni + cx, j + cy);
            leds[idx] += CHSV(color_shift + d4, 200, 192);
            idx = XY(j + cx, i + cy);
            leds[idx] += CHSV(color_shift + d5, 200, 192);
            idx = XY(nj + cx, ni + cy);
            leds[idx] += CHSV(color_shift + d6, 200, 192);

            // При соединении матрицы из угла вверх или вниз почему-то слева и справа узора остаются полосы, которые
            // не гаснут обычным blur - гасим полоски левой и правой стороны дополнительно.
            // При соединении из угла влево или вправо или на неквадратных матрицах такого эффекта не наблюдается
            for (byte i2 = cy; i2 < cy + seg_size; i2++)
            {
                fadePixel(cx, i2, 15);
                fadePixel(cx + BorderWidth, i2, 15);
                fadePixel(cx + seg_size - 1, i2, 15);
                fadePixel(cx + seg_size - BorderWidth - 1, i2, 15);
            }
        }
    }
    else
    {
        // Use two out-of-sync sine waves
        uint8_t i = beatsin8(41, BorderWidth, WIDTH - BorderWidth - 1);
        uint8_t j = beatsin8(27, BorderWidth, HEIGHT - BorderWidth - 1);

        // Also calculate some reflections
        uint8_t ni = (WIDTH - 1) - i;
        uint8_t nj = (HEIGHT - 1) - j;

        // The color of each point shifts over time, each at a different speed.
        idx = XY(i, j);
        leds[idx] += CHSV(ms / 11, 200, 192);
        idx = XY(ni, nj);
        leds[idx] += CHSV(ms / 13, 200, 192);
        idx = XY(i, nj);
        leds[idx] += CHSV(ms / 17, 200, 192);
        idx = XY(ni, j);
        leds[idx] += CHSV(ms / 29, 200, 192);

        if (HEIGHT == WIDTH)
        {
            // для квадратных матриц - 6 точек создают более красивую картину
            idx = XY(j, i);
            leds[idx] += CHSV(ms / 37, 200, 192);
            idx = XY(nj, ni);
            leds[idx] += CHSV(ms / 41, 200, 192);

            // При соединении матрицы из угла вверх или вниз почему-то слева и справа узора остаются полосы, которые
            // не гаснут обычным blur - гасим полоски левой и правой стороны дополнительно.
            // При соединении из угла влево или вправо или на неквадратных матрицах такого эффекта не наблюдается
            for (byte i = 0; i < HEIGHT; i++)
            {
                fadePixel(0, i, 15);
                fadePixel(WIDTH - 1, i, 15);
            }
        }
    }
}

uint16_t XY(uint8_t x, uint8_t y)
{
    return getPixelNumber(x, y);
}

//--------------------крутящаяся радуга матрица------------------

void rainbow_loop_matrix()
{ //-m3-LOOP HSV RAINBOW
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
    LEDS.show();
    if (safeDelay(thisdelay))
        return;
}