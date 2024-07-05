/*
#include "Global.h"
#include "classes/IoTItem.h"



#include "dial.h"
//#include "gauge2.h"
#include "NotoSansBold36.h"

// This example draws an animated dial with a rotating needle.

// The dial is a jpeg image, the needle is created using a rotated
// Sprite. The example operates by reading blocks of pixels from the
// TFT, thus the TFT setup must support reading from the TFT CGRAM.

// The sketch operates by creating a copy of the screen block where
// the needle will be drawn, the needle is then drawn on the screen.
// When the needle moves, the original copy of the screen area is
// pushed to the screen to over-write the needle graphic. A copy
// of the screen where the new position will be drawn is then made
// before drawing the needle in the new position. This technique
// allows the needle to move over other screen graphics.

// The sketch calculates the size of the buffer memory required and
// reserves the memory for the TFT block copy.

// В этом примере показан анимированный циферблат с вращающейся стрелкой.

// Циферблат представляет собой изображение в формате jpeg, стрелка создана с помощью повернутого
// Спрайт. В примере используется чтение блоков пикселей из
// TFT, поэтому настройка TFT должна поддерживать чтение из TFT CGRAM.

// Скетч работает, создавая копию блока экрана, где
// игла будет нарисована, затем игла будет нарисована на экране.
// При движении стрелки исходная копия области экрана
// выведено на экран, чтобы перезаписать изображение стрелки. Копия
// экрана, где будет отрисовываться новая позиция, затем создается
// перед рисованием стрелки в новом положении. Эта техника
// позволяет стрелке перемещаться по другой экранной графике.

// Скетч вычисляет размер требуемой буферной памяти и
// резервирует память для копии блока TFT.

// Created by Bodmer 17/3/20 as an example to the TFT_eSPI library:
// https://github.com/Bodmer/TFT_eSPI

#define POT_PIN 3
//#define POT_PIN A0

#define NEEDLE_LENGTH 35  // Visible length Видимая длина иглы
#define NEEDLE_WIDTH   5  // Width of needle - make it an odd number Ширина иглы - сделайте нечетное число
#define NEEDLE_RADIUS 90  // Radius at tip Радиус на кончике
#define NEEDLE_COLOR1 TFT_MAROON  // Needle periphery colour Цвет периферии иглы
#define NEEDLE_COLOR2 TFT_RED     // Needle centre colour    Цвет центра иглы
#define DIAL_CENTRE_X 120         //НАБОР ЦЕНТРА X
#define DIAL_CENTRE_Y 120

// Font attached to this sketch
// Шрифт прикреплен к этому эскизу

#define AA_FONT_LARGE NotoSansBold36

#include <Arduino.h>

#include <TFT_eSPI.h>
#include <SPI.h>
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite needle = TFT_eSprite(&tft); // Sprite object for needle Спрайт-объект для иглы
TFT_eSprite spr    = TFT_eSprite(&tft); // Sprite for meter reading Спрайт для чтения счетчика

// Jpeg image array attached to this sketch
// Массив изображений Jpeg, прикрепленный к этому эскизу

//#include "dial.h"

// Include the jpeg decoder library
// Включите библиотеку декодера jpeg

#include <TJpg_Decoder.h>

uint16_t* tft_buffer;
bool      buffer_loaded = false;
uint16_t  spr_width = 0;
uint16_t  bg_color =0;
// =======================================================================================
// This function will be called during decoding of the jpeg file
// Эта функция будет вызываться во время декодирования файла jpeg.
// =======================================================================================
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
  // Stop further decoding as image is running off bottom of screen
  // Остановить дальнейшее декодирование, так как изображение выходит за нижнюю часть экрана
  
  if ( y >= tft.height() ) return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
  // Эта функция автоматически обрезает рендеринг блока изображения на границах TFT.
  tft.pushImage(x, y, w, h, bitmap);

  // Return 1 to decode next block
  // Вернуть 1 для декодирования следующего блока

  return 1;
}

#include <map>


class Gc9A : public IoTItem {
   private:


    unsigned int _x;
    unsigned int _y;
    unsigned int _x1;
    unsigned int _y1;
    String _id2show;
    String _descr;
    String _descr1;
    String _colour;
    String _fon;
    int _colour1 = TFT_WHITE;
    int _fon1 = TFT_BLACK ;
    int _shrift1 = 2;
    int _shrift = 2;
    int _prevStrSize = 0;
    int _prevStrSize1 = 0;
    int _rotation = 0;
    int _rotation1 = 0;
    int _size;
    String _fon_screen;
    int _fon_screen1 = TFT_BLACK;
    String _angle;



    bool _isShow = true;    // экран показывает



   public:

   
    Gc9A(String parameters) : IoTItem(parameters) {

          String  xy;

    if (_size == 0){
            
         
            jsonRead(parameters, "size", _size);
            if (_size == 144 ) tft.begin();   

        jsonRead(parameters, "rotation", _rotation);
        if (_rotation == 2) _rotation1 = 2;
        else if (_rotation == 3) _rotation1 = 3;
        else if (_rotation == 1) _rotation1 = 1;
        else  _rotation1 = 0;
        tft.setRotation(_rotation1);    //Landscape
         }
        jsonRead(parameters, "fon_screen", _fon_screen);

        if (_fon_screen == "YELLOW") _fon_screen1 = TFT_YELLOW;
              else if (_fon_screen == "RED") _fon_screen1 = TFT_RED;
              else if (_fon_screen == "GREEN") _fon_screen1 = TFT_GREEN;
              else if (_fon_screen == "BLUE") _fon_screen1 = TFT_BLUE;
              else if (_fon_screen == "WHITE") _fon_screen1 = TFT_WHITE;
              else if (_fon_screen == "CYAN") _fon_screen1 = TFT_CYAN;
              else if (_fon_screen == "MAGENTA") _fon_screen1 = TFT_MAGENTA;
              else if (_fon_screen == "ORANGE") _fon_screen1 = TFT_ORANGE;
              else  _fon_screen1 = TFT_BLACK;

        tft.fillScreen(_fon_screen1);
 

        Serial.println(F("Initialized"));
    
               

        jsonRead(parameters, "coord", xy);
        _x = selectFromMarkerToMarker(xy, ",", 0).toInt();
        _y = selectFromMarkerToMarker(xy, ",", 1).toInt();

        jsonRead(parameters, "descr", _descr);
        jsonRead(parameters, "id2show", _id2show);
        jsonRead(parameters, "descr1", _descr1);
        jsonRead(parameters, "shrift", _shrift);
        jsonRead(parameters, "colour", _colour);
        jsonRead(parameters, "fon", _fon);

        // The byte order can be swapped (set true for TFT_eSPI)
  // Порядок байтов можно поменять местами (установите true для TFT_eSPI)
  TJpgDec.setSwapBytes(true);

  // The jpeg decoder must be given the exact name of the rendering function above
  // Декодеру jpeg должно быть присвоено точное имя функции рендеринга выше.
  TJpgDec.setCallback(tft_output);

  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  // Draw the dial Нарисуйте циферблат
  TJpgDec.drawJpg(0, 0, dial, sizeof(dial));
   //TJpgDec.drawJpg(0, 0, gauge2, sizeof(gauge2));
  tft.drawCircle(DIAL_CENTRE_X, DIAL_CENTRE_Y, NEEDLE_RADIUS-NEEDLE_LENGTH, TFT_DARKGREY);

  // Load the font and create the Sprite for reporting the value
  // Загружаем шрифт и создаем спрайт для сообщения значения
  spr.loadFont(AA_FONT_LARGE);
  spr_width = spr.textWidth("777"); // 7 is widest numeral in this font 7 — самая широкая цифра в этом шрифте
  //spr_width = spr.textWidth(_id2show); // 7 is widest numeral in this font 7 — самая широкая цифра в этом шрифте
  spr.createSprite(spr_width, spr.fontHeight());
  bg_color = tft.readPixel(120, 120); // Get colour from dial centre Получить цвет из центра циферблата
  spr.fillSprite(bg_color);
  spr.setTextColor(TFT_WHITE, bg_color, true);
  spr.setTextDatum(MC_DATUM);
  spr.setTextPadding(spr_width);
  spr.drawNumber(0, spr_width/2, spr.fontHeight()/2);
  spr.pushSprite(DIAL_CENTRE_X - spr_width / 2, DIAL_CENTRE_Y - spr.fontHeight() / 2);

  // Plot the label text Нанесите текст метки
  tft.setTextColor(TFT_WHITE, bg_color);
  tft.setTextDatum(MC_DATUM);
  //tft.drawString("(degrees)", DIAL_CENTRE_X, DIAL_CENTRE_Y + 48, 2);
  tft.drawString("IoTmanager", DIAL_CENTRE_X, DIAL_CENTRE_Y + 48, 2);

  // Define where the needle pivot point is on the TFT before
  // creating the needle so boundary calculation is correct
  // Определяем, где находится точка поворота стрелки на TFT перед
  // создаем иглу, чтобы расчет границы был правильным
  tft.setPivot(DIAL_CENTRE_X, DIAL_CENTRE_Y);

  // Create the needle Sprite
  // Создаем игольчатый спрайт
  createNeedle();

  // Reset needle position to 0
  // Сбросить положение иглы на 0
  plotNeedle(0, 0);

  

    }

    

    void doByInterval() {

                  String tmpStr = "";

                           //uint16_t angle = getItemValue(_id2show);
//uint16_t angle = random(241); // random speed in range 0 to 240
                           uint16_t angle = map(analogRead(POT_PIN),0,4095,0,240);
                           //uint16_t angle = getItemValue(_id2show);

                          // uint16_t angle = value;
                          // String angle = getItemValue(_id2show);
                             
  // Plot needle at random angle in range 0 to 240, speed 40ms per increment
  // Построение стрелки под случайным углом в диапазоне от 0 до 240, скорость 40 мс на приращение
  plotNeedle(angle, 30); 
    //plotNeedle(getItemValue(_id2show), 30); //=================================
      float plotNeedle(float value);
            
            if (_descr != "none") tmpStr = _descr + " " + getItemValue(_id2show) + " " + _descr1;
                else tmpStr = getItemValue(_id2show);

            

            tft.setTextWrap(false);

            tft.setCursor(_x, _y);


             if (_fon == "YELLOW") _fon1 = TFT_YELLOW;
              else if (_fon == "RED") _fon1 = TFT_RED;
              else if (_fon == "GREEN") _fon1 = TFT_GREEN;
              else if (_fon == "BLUE") _fon1 = TFT_BLUE;
              else if (_fon == "WHITE") _fon1 = TFT_WHITE;
              else if (_fon == "CYAN") _fon1 = TFT_CYAN;
              else if (_fon == "MAGENTA") _fon1 = TFT_MAGENTA;
              else if (_fon == "ORANGE") _fon1 = TFT_ORANGE;
              else  _fon1 = TFT_BLACK;   
            
                 
              if (_colour == "YELLOW")  _colour1 = TFT_YELLOW;
               else if (_colour == "RED") _colour1 = TFT_RED ;
               else if (_colour == "GREEN") _colour1 = TFT_GREEN;
               else if (_colour == "BLUE") _colour1 = TFT_BLUE;
               else if (_colour == "BLACK") _colour1 = TFT_BLACK;
               else if (_colour == "CYAN") _colour1 = TFT_CYAN;
               else if (_colour == "MAGENTA") _colour1 = TFT_MAGENTA;
               else if (_colour == "ORANGE") _colour1 = TFT_ORANGE;
               else _colour1 = TFT_WHITE;
               
              
               tft.setTextColor(_colour1, _fon_screen1);
          

          _prevStrSize1 = tmpStr.length();
          if (_prevStrSize > _prevStrSize1){

            String _tmpStr = "";

            int strSize = _prevStrSize - _prevStrSize1;

            for (int i = 0; i < strSize; i++) _tmpStr += " ";

            tft.setTextSize(_shrift);
             
            tft.setCursor (_prevStrSize1 * 6 * _shrift + _x , _y);
            
            tft.print(_tmpStr);
          }
        if ( _shrift1 != _shrift || _x1 != _x || _y1 != _y){
          
                    tft.setCursor(_x1, _y1);
                    tft.setTextSize(_shrift1);
                    String _tmpStr = "";
                    for (int i = 0; i < _prevStrSize; i++) _tmpStr += " ";
                    
                    tft.print(_tmpStr);
                   
                    _shrift1 = _shrift;
                    _x1 = _x;
                    _y1 = _y;
                    }
          
          tft.setTextColor(_colour1, _fon1); 
          tft.setTextSize(_shrift);
          tft.setCursor(_x, _y);

          tft.print(tmpStr);

          _prevStrSize = tmpStr.length();



    }

// =======================================================================================
// Create the needle Sprite
// Создаем игольчатый спрайт
// =======================================================================================
void createNeedle(void)
{
  needle.setColorDepth(16);
  needle.createSprite(NEEDLE_WIDTH, NEEDLE_LENGTH);  // create the needle Sprite создаем игольчатый спрайт

  needle.fillSprite(TFT_BLACK); // Fill with black Залейте черным

  // Define needle pivot point relative to top left corner of Sprite
  // Определяем точку поворота стрелки относительно верхнего левого угла спрайта
  uint16_t piv_x = NEEDLE_WIDTH / 2; // pivot x in Sprite (middle)стержень x в Sprite (средний)
  uint16_t piv_y = NEEDLE_RADIUS;    // pivot y in Sprite
  needle.setPivot(piv_x, piv_y);     // Set pivot point in this Sprite Установить точку разворота в этом спрайте

  // Draw the red needle in the Sprite
  // Рисуем красную иглу в Спрайте
  needle.fillRect(0, 0, NEEDLE_WIDTH, NEEDLE_LENGTH, TFT_MAROON);
  needle.fillRect(1, 1, NEEDLE_WIDTH-2, NEEDLE_LENGTH-2, TFT_RED);

  // Bounding box parameters to be populated
  // Параметры ограничивающей рамки для заполнения
  int16_t min_x;
  int16_t min_y;
  int16_t max_x;
  int16_t max_y;

  // Work out the worst case area that must be grabbed from the TFT,
  // this is at a 45 degree rotation
  // Вычисляем область наихудшего случая, которую необходимо захватить из TFT,
  // это при повороте на 45 градусов
  needle.getRotatedBounds(45, &min_x, &min_y, &max_x, &max_y);

  // Calculate the size and allocate the buffer for the grabbed TFT area
  // Рассчитываем размер и выделяем буфер для захваченной области TFT
  tft_buffer =  (uint16_t*) malloc( ((max_x - min_x) + 2) * ((max_y - min_y) + 2) * 2 );
}

// =======================================================================================
// Move the needle to a new position
// Перемещаем иглу в новое положение
// =======================================================================================
void plotNeedle(int16_t angle, uint16_t ms_delay)
{
  static int16_t old_angle = -120; // Starts at -120 degrees Начинается при -120 градусов

  // Bounding box parameters
  // Параметры ограничивающей рамки
  static int16_t min_x;
  static int16_t min_y;
  static int16_t max_x;
  static int16_t max_y;

  if (angle < 0) angle = 0; // Limit angle to emulate needle end stops Ограничение угла для имитации концевых упоров иглы
  if (angle > 240) angle = 240;

  angle -= 120; // Starts at -120 degrees Начинается при -120 градусов

  // Move the needle until new angle reached
  // Двигаем иглу до достижения нового угла
  while (angle != old_angle || !buffer_loaded) {

    if (old_angle < angle) old_angle++;
    else old_angle--;

    // Only plot needle at even values to improve plotting performance
    // Строить стрелку только при четных значениях, чтобы улучшить производительность построения графика
    if ( (old_angle & 1) == 0)
    {
      if (buffer_loaded) {
        // Paste back the original needle free image area
        // Вставляем обратно исходную область изображения без иглы
        tft.pushRect(min_x, min_y, 1 + max_x - min_x, 1 + max_y - min_y, tft_buffer);
      }

      if ( needle.getRotatedBounds(old_angle, &min_x, &min_y, &max_x, &max_y) )
      {
        // Grab a copy of the area before needle is drawn
        // Захватите копию области до того, как будет нарисована игла
        tft.readRect(min_x, min_y, 1 + max_x - min_x, 1 + max_y - min_y, tft_buffer);
        buffer_loaded = true;
      }

      // Draw the needle in the new position, black in needle image is transparent
      // Рисуем иглу в новом положении, черный цвет на изображении иглы прозрачен
      needle.pushRotated(old_angle, TFT_BLACK);

      // Wait before next update
      // Подождем до следующего обновления
      delay(ms_delay);
    }

    // Update the number at the centre of the dial
    // Обновляем число в центре циферблата
    spr.setTextColor(TFT_WHITE, bg_color, true);
    spr.drawNumber(old_angle+120, spr_width/2, spr.fontHeight()/2);

    //spr.drawNumber(getItemValue(_id2show), spr_width/2, spr.fontHeight()/2);//=========================

    //getItemValue(_id2show)
    spr.pushSprite(120 - spr_width / 2, 120 - spr.fontHeight() / 2);

    // Slow needle down slightly as it approaches the new position
    // Слегка замедляем иглу, когда она приближается к новому положению
    if (abs(old_angle - angle) < 10) ms_delay += ms_delay / 5;
  }
}


    IoTValue execute(String command, std::vector<IoTValue> &param) {  // будет возможным использовать, когда сценарии запустятся
               if (command == "x") {
            if (param.size()) {
                _x = param[0].valD;
            }
        } else if (command == "y") {
            if (param.size()) {
                _y = param[0].valD;
            }
        } else if (command == "descr") {
            if (param.size()) {
                _descr = param[0].valS;
            }
        } else if (command == "id2show") {
            if (param.size()) {
                _id2show = param[0].valS;
            }
        } else if (command == "shrift") {
                if (param.size()) {
                _shrift = param[0].valD;
            }
        } else if (command == "xy") {
            if (param.size()) {
                _x = param[0].valD;
                _y = param[1].valD;
            }
        }   else if (command == "colour") {
                if (param.size()) {
                _colour = param[0].valS;
                }
        } else if (command == "xycs") {
            if (param.size()) {
                _x = param[0].valD;
                _y = param[1].valD;
                _colour = param[2].valS;
                _shrift = param[3].valD;
            }
        } else if (command == "descr1") {
            if (param.size()) {
                _descr1 = param[0].valS;
            }
        } else if (command == "fon") {
                if (param.size()) {
                _fon = param[0].valS;
                }
        } else if (command == "line") {
            if (param.size()) {
                int a = param[0].valD;
                int b = param[1].valD;
                int c = param[2].valD;
                int d = param[3].valD;
                String _colour2 = param[4].valS;
                int _colour21;
                if (_colour2 == "YELLOW")  _colour21 = TFT_YELLOW;
               else if (_colour2 == "RED") _colour21 = TFT_RED ;
               else if (_colour2 == "GREEN") _colour21 = TFT_GREEN;
               else if (_colour2 == "BLUE") _colour21 = TFT_BLUE;
               else if (_colour2 == "BLACK") _colour21 = TFT_BLACK;
               else if (_colour2 == "CYAN") _colour21 = TFT_CYAN;
               else if (_colour2 == "MAGENTA") _colour21 = TFT_MAGENTA;
               else if (_colour2 == "ORANGE") _colour21 = TFT_ORANGE;
               else _colour21 = TFT_WHITE;
               
               tft.drawLine(a, b, c, d, _colour21);
            }
        }


        doByInterval();
        return {};

               
        }

    ~Gc9A(){};
};

void *getAPI_Gc9A(String subtype, String param) {
    if (subtype == F("Gc9A")) {
        return new Gc9A(param);
    } else {
        return nullptr;
    }
}
*/
#include "Global.h"
#include "classes/IoTItem.h"

#include "dial.h"
//#include "gauge2.h"
#include "NotoSansBold36.h"

// This example draws an animated dial with a rotating needle.

// The dial is a jpeg image, the needle is created using a rotated
// Sprite. The example operates by reading blocks of pixels from the
// TFT, thus the TFT setup must support reading from the TFT CGRAM.

// The sketch operates by creating a copy of the screen block where
// the needle will be drawn, the needle is then drawn on the screen.
// When the needle moves, the original copy of the screen area is
// pushed to the screen to over-write the needle graphic. A copy
// of the screen where the new position will be drawn is then made
// before drawing the needle in the new position. This technique
// allows the needle to move over other screen graphics.

// The sketch calculates the size of the buffer memory required and
// reserves the memory for the TFT block copy.

// В этом примере показан анимированный циферблат с вращающейся стрелкой.

// Циферблат представляет собой изображение в формате jpeg, стрелка создана с помощью повернутого
// Спрайт. В примере используется чтение блоков пикселей из
// TFT, поэтому настройка TFT должна поддерживать чтение из TFT CGRAM.

// Скетч работает, создавая копию блока экрана, где
// игла будет нарисована, затем игла будет нарисована на экране.
// При движении стрелки исходная копия области экрана
// выведено на экран, чтобы перезаписать изображение стрелки. Копия
// экрана, где будет отрисовываться новая позиция, затем создается
// перед рисованием стрелки в новом положении. Эта техника
// позволяет стрелке перемещаться по другой экранной графике.

// Скетч вычисляет размер требуемой буферной памяти и
// резервирует память для копии блока TFT.

// Created by Bodmer 17/3/20 as an example to the TFT_eSPI library:
// https://github.com/Bodmer/TFT_eSPI

//#define POT_PIN 3
// #define POT_PIN A0

#define NEEDLE_LENGTH 35         // Visible length Видимая длина иглы
#define NEEDLE_WIDTH 5           // Width of needle - make it an odd number Ширина иглы - сделайте нечетное число
#define NEEDLE_RADIUS 90         // Radius at tip Радиус на кончике
#define NEEDLE_COLOR1 TFT_MAROON // Needle periphery colour Цвет периферии иглы
#define NEEDLE_COLOR2 TFT_RED    // Needle centre colour    Цвет центра иглы
#define DIAL_CENTRE_X 120        // НАБОР ЦЕНТРА X
#define DIAL_CENTRE_Y 120

// Font attached to this sketch
// Шрифт прикреплен к этому эскизу

#define AA_FONT_LARGE NotoSansBold36

#include <Arduino.h>

#include <TFT_eSPI.h>
#include <SPI.h>
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite needle = TFT_eSprite(&tft); // Sprite object for needle Спрайт-объект для иглы
TFT_eSprite spr = TFT_eSprite(&tft);    // Sprite for meter reading Спрайт для чтения счетчика

// Jpeg image array attached to this sketch
// Массив изображений Jpeg, прикрепленный к этому эскизу

// #include "dial.h"

// Include the jpeg decoder library
// Включите библиотеку декодера jpeg

#include <TJpg_Decoder.h>

uint16_t *tft_buffer;
bool buffer_loaded = false;
uint16_t spr_width = 0;
uint16_t bg_color = 0;
// =======================================================================================
// This function will be called during decoding of the jpeg file
// Эта функция будет вызываться во время декодирования файла jpeg.
// =======================================================================================
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap)
{
    // Stop further decoding as image is running off bottom of screen
    // Остановить дальнейшее декодирование, так как изображение выходит за нижнюю часть экрана

    if (y >= tft.height())
        return 0;

    // This function will clip the image block rendering automatically at the TFT boundaries
    // Эта функция автоматически обрезает рендеринг блока изображения на границах TFT.
    tft.pushImage(x, y, w, h, bitmap);

    // Return 1 to decode next block
    // Вернуть 1 для декодирования следующего блока

    return 1;
}

#include <map>

class Gc9A : public IoTItem
{
private:
    unsigned int _x;
    unsigned int _y;
    unsigned int _x1;
    unsigned int _y1;

    unsigned int _c;
    unsigned int _d;
    unsigned int _e;
    unsigned int _f;

    String _id2show;
    String _descr;
    String _descr1;
    String _colour;
    String _fon;
    int _colour1 = TFT_WHITE;
    int _fon1 = TFT_BLACK;
    int _shrift1 = 2;
    int _shrift = 2;
    int _prevStrSize = 0;
    int _prevStrSize1 = 0;
    int _rotation = 0;
    int _rotation1 = 0;
    int _size;
    String _fon_screen;
    int _fon_screen1 = TFT_BLACK;
    String _angle;

    bool _isShow = true; // экран показывает

public:
    Gc9A(String parameters) : IoTItem(parameters)
    {

        String xy;

        String cdef;

        if (_size == 0)
        {

            jsonRead(parameters, "size", _size);
            if (_size == 144)
                tft.begin();

            jsonRead(parameters, "rotation", _rotation);
            if (_rotation == 2)
                _rotation1 = 2;
            else if (_rotation == 3)
                _rotation1 = 3;
            else if (_rotation == 1)
                _rotation1 = 1;
            else
                _rotation1 = 0;
            tft.setRotation(_rotation1); // Landscape
        }
        jsonRead(parameters, "fon_screen", _fon_screen);

        if (_fon_screen == "YELLOW")
            _fon_screen1 = TFT_YELLOW;
        else if (_fon_screen == "RED")
            _fon_screen1 = TFT_RED;
        else if (_fon_screen == "GREEN")
            _fon_screen1 = TFT_GREEN;
        else if (_fon_screen == "BLUE")
            _fon_screen1 = TFT_BLUE;
        else if (_fon_screen == "WHITE")
            _fon_screen1 = TFT_WHITE;
        else if (_fon_screen == "CYAN")
            _fon_screen1 = TFT_CYAN;
        else if (_fon_screen == "MAGENTA")
            _fon_screen1 = TFT_MAGENTA;
        else if (_fon_screen == "ORANGE")
            _fon_screen1 = TFT_ORANGE;
        else
            _fon_screen1 = TFT_BLACK;

        tft.fillScreen(_fon_screen1);

        Serial.println(F("Initialized"));

        jsonRead(parameters, "coord", xy);
        _x = selectFromMarkerToMarker(xy, ",", 0).toInt();
        _y = selectFromMarkerToMarker(xy, ",", 1).toInt();

        jsonRead(parameters, "descr", _descr);
        jsonRead(parameters, "id2show", _id2show);
        jsonRead(parameters, "descr1", _descr1);
        jsonRead(parameters, "shrift", _shrift);
        jsonRead(parameters, "colour", _colour);
        jsonRead(parameters, "fon", _fon);

        jsonRead(parameters, "map", cdef);
        _c = selectFromMarkerToMarker(cdef, ",", 0).toInt();
        _d = selectFromMarkerToMarker(cdef, ",", 1).toInt();
        _e = selectFromMarkerToMarker(cdef, ",", 2).toInt();
        _f = selectFromMarkerToMarker(cdef, ",", 3).toInt();


        // The byte order can be swapped (set true for TFT_eSPI)
        // Порядок байтов можно поменять местами (установите true для TFT_eSPI)
        TJpgDec.setSwapBytes(true);

        // The jpeg decoder must be given the exact name of the rendering function above
        // Декодеру jpeg должно быть присвоено точное имя функции рендеринга выше.
        TJpgDec.setCallback(tft_output);

        tft.begin();
        tft.setRotation(0);
        tft.fillScreen(TFT_BLACK);

        // Draw the dial Нарисуйте циферблат
        TJpgDec.drawJpg(0, 0, dial, sizeof(dial));
         //TJpgDec.drawJpg(0, 0, gauge2, sizeof(gauge2));
        tft.drawCircle(DIAL_CENTRE_X, DIAL_CENTRE_Y, NEEDLE_RADIUS - NEEDLE_LENGTH, TFT_DARKGREY);

        // Load the font and create the Sprite for reporting the value
        // Загружаем шрифт и создаем спрайт для сообщения значения
        spr.loadFont(AA_FONT_LARGE);
        spr_width = spr.textWidth("777"); // 7 is widest numeral in this font 7 — самая широкая цифра в этом шрифте
        spr.createSprite(spr_width, spr.fontHeight());
        bg_color = tft.readPixel(120, 120); // Get colour from dial centre Получить цвет из центра циферблата
        spr.fillSprite(bg_color);
        spr.setTextColor(TFT_WHITE, bg_color, true);
        spr.setTextDatum(MC_DATUM);
        spr.setTextPadding(spr_width);
        spr.drawNumber(0, spr_width / 2, spr.fontHeight() / 2);
        spr.pushSprite(DIAL_CENTRE_X - spr_width / 2, DIAL_CENTRE_Y - spr.fontHeight() / 2);

        // Plot the label text Нанесите текст метки
        tft.setTextColor(TFT_WHITE, bg_color);
        tft.setTextDatum(MC_DATUM);
        // tft.drawString("(degrees)", DIAL_CENTRE_X, DIAL_CENTRE_Y + 48, 2);
        tft.drawString("IoTman", DIAL_CENTRE_X, DIAL_CENTRE_Y + 48, 2);

        // Define where the needle pivot point is on the TFT before
        // creating the needle so boundary calculation is correct
        // Определяем, где находится точка поворота стрелки на TFT перед
        // создаем иглу, чтобы расчет границы был правильным
        tft.setPivot(DIAL_CENTRE_X, DIAL_CENTRE_Y);

        // Create the needle Sprite
        // Создаем игольчатый спрайт
        createNeedle();

        // Reset needle position to 0
        // Сбросить положение иглы на 0
        plotNeedle(0, 0);
    }

    void doByInterval()
    {

        String tmpStr = "";

        uint16_t angle;// = map(analogRead(POT_PIN), 0, 4095, 0, 240);

        IoTItem* tmp = findIoTItem(_id2show);
        if (tmp)
            angle = (uint16_t)tmp->value.valD;

        angle = constrain(angle, 0, 50); //привести к мин и макс темппературе, пример: если больше 50, то будет 50
        angle = map(angle, 0, 50, 0, 240);  // привести к углу, 0темп=0угол .... 50темп=240угол  
        // uint16_t angle = value;
        // String angle = getItemValue(_id2show);

        // Plot needle at random angle in range 0 to 240, speed 40ms per increment
        // Построение стрелки под случайным углом в диапазоне от 0 до 240, скорость 40 мс на приращение
        plotNeedle(angle, 30);
        // plotNeedle(getItemValue(_id2show), 30); //=================================
        //   float plotNeedle(float value);

        if (_descr != "none")
            tmpStr = _descr + " " + getItemValue(_id2show) + " " + _descr1;
        else
            tmpStr = getItemValue(_id2show);

        tft.setTextWrap(false);

        tft.setCursor(_x, _y);

        if (_fon == "YELLOW")
            _fon1 = TFT_YELLOW;
        else if (_fon == "RED")
            _fon1 = TFT_RED;
        else if (_fon == "GREEN")
            _fon1 = TFT_GREEN;
        else if (_fon == "BLUE")
            _fon1 = TFT_BLUE;
        else if (_fon == "WHITE")
            _fon1 = TFT_WHITE;
        else if (_fon == "CYAN")
            _fon1 = TFT_CYAN;
        else if (_fon == "MAGENTA")
            _fon1 = TFT_MAGENTA;
        else if (_fon == "ORANGE")
            _fon1 = TFT_ORANGE;
        else
            _fon1 = TFT_BLACK;

        if (_colour == "YELLOW")
            _colour1 = TFT_YELLOW;
        else if (_colour == "RED")
            _colour1 = TFT_RED;
        else if (_colour == "GREEN")
            _colour1 = TFT_GREEN;
        else if (_colour == "BLUE")
            _colour1 = TFT_BLUE;
        else if (_colour == "BLACK")
            _colour1 = TFT_BLACK;
        else if (_colour == "CYAN")
            _colour1 = TFT_CYAN;
        else if (_colour == "MAGENTA")
            _colour1 = TFT_MAGENTA;
        else if (_colour == "ORANGE")
            _colour1 = TFT_ORANGE;
        else
            _colour1 = TFT_WHITE;

        tft.setTextColor(_colour1, _fon_screen1);

        _prevStrSize1 = tmpStr.length();
        if (_prevStrSize > _prevStrSize1)
        {

            String _tmpStr = "";

            int strSize = _prevStrSize - _prevStrSize1;

            for (int i = 0; i < strSize; i++)
                _tmpStr += " ";

            tft.setTextSize(_shrift);

            tft.setCursor(_prevStrSize1 * 6 * _shrift + _x, _y);

            tft.print(_tmpStr);
        }
        if (_shrift1 != _shrift || _x1 != _x || _y1 != _y)
        {

            tft.setCursor(_x1, _y1);
            tft.setTextSize(_shrift1);
            String _tmpStr = "";
            for (int i = 0; i < _prevStrSize; i++)
                _tmpStr += " ";

            tft.print(_tmpStr);

            _shrift1 = _shrift;
            _x1 = _x;
            _y1 = _y;
        }

        tft.setTextColor(_colour1, _fon1);
        tft.setTextSize(_shrift);
        tft.setCursor(_x, _y);

        tft.print(tmpStr);
        spr.setTextColor(TFT_WHITE, bg_color, true);

        if(tmp)
        spr.drawNumber(tmp->value.valD, spr_width/2, spr.fontHeight()/2);

            // Обновляем число в центре циферблата
   // spr.setTextColor(TFT_WHITE, bg_color, true);
   // spr.drawNumber(old_angle+120, spr_width/2, spr.fontHeight()/2);
        spr.pushSprite(DIAL_CENTRE_X - spr_width / 2, DIAL_CENTRE_Y - spr.fontHeight() / 2);
       // spr.drawNumber(getItemValue(_id2show), spr_width/2, spr.fontHeight()/2);
        //spr.drawNumber(old_angle+120, spr_width/2, spr.fontHeight()/2);
 //spr.drawNumber(0, spr_width / 2, spr.fontHeight() / 2);
        _prevStrSize = tmpStr.length();
    }

    // =======================================================================================
    // Create the needle Sprite
    // Создаем игольчатый спрайт
    // =======================================================================================
    void createNeedle(void)
    {
        needle.setColorDepth(16);
        needle.createSprite(NEEDLE_WIDTH, NEEDLE_LENGTH); // create the needle Sprite создаем игольчатый спрайт

        needle.fillSprite(TFT_BLACK); // Fill with black Залейте черным

        // Define needle pivot point relative to top left corner of Sprite
        // Определяем точку поворота стрелки относительно верхнего левого угла спрайта
        uint16_t piv_x = NEEDLE_WIDTH / 2; // pivot x in Sprite (middle)стержень x в Sprite (средний)
        uint16_t piv_y = NEEDLE_RADIUS;    // pivot y in Sprite
        needle.setPivot(piv_x, piv_y);     // Set pivot point in this Sprite Установить точку разворота в этом спрайте

        // Draw the red needle in the Sprite
        // Рисуем красную иглу в Спрайте
        needle.fillRect(0, 0, NEEDLE_WIDTH, NEEDLE_LENGTH, TFT_MAROON);
        needle.fillRect(1, 1, NEEDLE_WIDTH - 2, NEEDLE_LENGTH - 2, TFT_RED);

        // Bounding box parameters to be populated
        // Параметры ограничивающей рамки для заполнения
        int16_t min_x;
        int16_t min_y;
        int16_t max_x;
        int16_t max_y;

        // Work out the worst case area that must be grabbed from the TFT,
        // this is at a 45 degree rotation
        // Вычисляем область наихудшего случая, которую необходимо захватить из TFT,
        // это при повороте на 45 градусов
        needle.getRotatedBounds(45, &min_x, &min_y, &max_x, &max_y);

        // Calculate the size and allocate the buffer for the grabbed TFT area
        // Рассчитываем размер и выделяем буфер для захваченной области TFT
        tft_buffer = (uint16_t *)malloc(((max_x - min_x) + 2) * ((max_y - min_y) + 2) * 2);
    }

    // =======================================================================================
    // Move the needle to a new position
    // Перемещаем иглу в новое положение
    // =======================================================================================
    void plotNeedle(int16_t angle, uint16_t ms_delay)
    {
        static int16_t old_angle = -120; // Starts at -120 degrees Начинается при -120 градусов

        // Bounding box parameters
        // Параметры ограничивающей рамки
        static int16_t min_x;
        static int16_t min_y;
        static int16_t max_x;
        static int16_t max_y;

        if (angle < 0)
            angle = 0; // Limit angle to emulate needle end stops Ограничение угла для имитации концевых упоров иглы
        if (angle > 240)
            angle = 240;

        angle -= 120; // Starts at -120 degrees Начинается при -120 градусов

        // Move the needle until new angle reached
        // Двигаем иглу до достижения нового угла
        while (angle != old_angle || !buffer_loaded)
        {

            if (old_angle < angle)
                old_angle++;
            else
                old_angle--;

            // Only plot needle at even values to improve plotting performance
            // Строить стрелку только при четных значениях, чтобы улучшить производительность построения графика
            if ((old_angle & 1) == 0)
            {
                if (buffer_loaded)
                {
                    // Paste back the original needle free image area
                    // Вставляем обратно исходную область изображения без иглы
                    tft.pushRect(min_x, min_y, 1 + max_x - min_x, 1 + max_y - min_y, tft_buffer);
                }

                if (needle.getRotatedBounds(old_angle, &min_x, &min_y, &max_x, &max_y))
                {
                    // Grab a copy of the area before needle is drawn
                    // Захватите копию области до того, как будет нарисована игла
                    tft.readRect(min_x, min_y, 1 + max_x - min_x, 1 + max_y - min_y, tft_buffer);
                    buffer_loaded = true;
                }

                // Draw the needle in the new position, black in needle image is transparent
                // Рисуем иглу в новом положении, черный цвет на изображении иглы прозрачен
                needle.pushRotated(old_angle, TFT_BLACK);

                // Wait before next update
                // Подождем до следующего обновления
                delay(ms_delay);
            }

            // Update the number at the centre of the dial
            // Обновляем число в центре циферблата
            //    spr.setTextColor(TFT_WHITE, bg_color, true);
            //    spr.drawNumber(old_angle+120, spr_width/2, spr.fontHeight()/2);

            // spr.drawNumber(getItemValue(_id2show), spr_width/2, spr.fontHeight()/2);//=========================

            // getItemValue(_id2show)
            spr.pushSprite(120 - spr_width / 2, 120 - spr.fontHeight() / 2);

            //spr.pushSprite(DIAL_CENTRE_X - spr_width / 2, DIAL_CENTRE_Y - spr.fontHeight() / 2);
 //spr.drawNumber(0, spr_width / 2, spr.fontHeight() / 2);
            // Slow needle down slightly as it approaches the new position
            // Слегка замедляем иглу, когда она приближается к новому положению
            if (abs(old_angle - angle) < 10)
                ms_delay += ms_delay / 5;
        }
    }

    IoTValue execute(String command, std::vector<IoTValue> &param)
    { // будет возможным использовать, когда сценарии запустятся
        if (command == "x")
        {
            if (param.size())
            {
                _x = param[0].valD;
            }
        }
        else if (command == "y")
        {
            if (param.size())
            {
                _y = param[0].valD;
            }
        }
        else if (command == "descr")
        {
            if (param.size())
            {
                _descr = param[0].valS;
            }
        }
        else if (command == "id2show")
        {
            if (param.size())
            {
                _id2show = param[0].valS;
            }
        }
        else if (command == "shrift")
        {
            if (param.size())
            {
                _shrift = param[0].valD;
            }
        }
        else if (command == "xy")
        {
            if (param.size())
            {
                _x = param[0].valD;
                _y = param[1].valD;
            }
        }
        else if (command == "colour")
        {
            if (param.size())
            {
                _colour = param[0].valS;
            }
        }
        else if (command == "xycs")
        {
            if (param.size())
            {
                _x = param[0].valD;
                _y = param[1].valD;
                _colour = param[2].valS;
                _shrift = param[3].valD;
            }
        }
        else if (command == "descr1")
        {
            if (param.size())
            {
                _descr1 = param[0].valS;
            }
        }
        else if (command == "fon")
        {
            if (param.size())
            {
                _fon = param[0].valS;
            }
        }
        else if (command == "line")
        {
            if (param.size())
            {
                int a = param[0].valD;
                int b = param[1].valD;
                int c = param[2].valD;
                int d = param[3].valD;
                String _colour2 = param[4].valS;
                int _colour21;
                if (_colour2 == "YELLOW")
                    _colour21 = TFT_YELLOW;
                else if (_colour2 == "RED")
                    _colour21 = TFT_RED;
                else if (_colour2 == "GREEN")
                    _colour21 = TFT_GREEN;
                else if (_colour2 == "BLUE")
                    _colour21 = TFT_BLUE;
                else if (_colour2 == "BLACK")
                    _colour21 = TFT_BLACK;
                else if (_colour2 == "CYAN")
                    _colour21 = TFT_CYAN;
                else if (_colour2 == "MAGENTA")
                    _colour21 = TFT_MAGENTA;
                else if (_colour2 == "ORANGE")
                    _colour21 = TFT_ORANGE;
                else
                    _colour21 = TFT_WHITE;

                tft.drawLine(a, b, c, d, _colour21);
            }
        }

        doByInterval();
        return {};
    }

    ~Gc9A(){};
};

void *getAPI_Gc9A(String subtype, String param)
{
    if (subtype == F("Gc9A"))
    {
        return new Gc9A(param);
    }
    else
    {
        return nullptr;
    }
}

