
// матрица
#define WIDTH 16   // ширина матрицы
#define HEIGHT 16  // высота матрицы
#define SEGMENTS 1 // диодов в одном "пикселе" (для создания матрицы из кусков ленты)
#define NUM_LEDS WIDTH *HEIGHT *SEGMENTS
// лента
#define LED_COUNT NUM_LEDS // число светодиодов в кольце/ленте
#define LED_DT 13          // пин, куда подключен DIN ленты