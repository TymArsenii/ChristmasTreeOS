/*
  ChristmasTreeOS by Arsenii Tymoshenko
*/

#include <FastLED.h>

#define NUM_LEDS 28
#define LED_PIN 13

#define HUE_START 3  // Starting color of fire -- not used!
#define HUE_GAP 18  // Color coefficient of fire
#define SMOOTH_K 0.15  // Smoothness coefficient of fire -- not used!
#define MIN_BRIGHT 80  // Minimum brightness of fire
#define MAX_BRIGHT 255  // Maximum brightness of fire
#define MIN_SAT 245  // Minimum saturation
#define MAX_SAT 255  // Maximum saturation

#define ZONE_AMOUNT NUM_LEDS
byte zoneValues[ZONE_AMOUNT];
byte zoneRndValues[ZONE_AMOUNT];

CRGB leds[NUM_LEDS];

#define FOR_i(from, to) for (int i = (from); i < (to); i++)
#define FOR_j(from, to) for (int j = (from); j < (to); j++)

byte change = 1;

float smooth_k_int = 0.25;
byte smooth_rainbow = 2;
byte min_bright = 25;
byte max_bright = 255;
byte fade_brightness;
byte smooth_fade = 40;
byte fill_color = 115;
byte fade_color = 115;
byte brightness_k = 20;
byte hue_start_int = 0;
byte saturation;
boolean flag;
boolean lcd_change;
boolean side;
byte cur_color = 0;
int new_comet_id;
int running_speed = 15;
int running_freq = 100;
bool draw_new_snow;
//unsigned long Ukraine_pro_timer;

byte red = 0;
byte green = 255;

volatile int ads;

int module;
float module_float;

byte color_pos[50][50];

uint32_t start_timer;
uint32_t button_debounce_timer;

unsigned long fade_timer = 0;
uint32_t running_timer;
uint32_t random_timer;

uint32_t loop_timer;
uint32_t christmas_lights_timer;
uint32_t snowflakes_timer;
uint32_t moving_ball_timer;

uint32_t change_timer;
byte snowflakes_amount;
byte snowflakes_pos[50];
byte snowflakes_id;

int ball_pos = 0;
int ball_dir = 1;
uint8_t ball_hue = 0;

bool color_flag;

byte balls_what_to_do;
uint32_t random_balls_timer;
byte random_balls_color_arr[NUM_LEDS + 1];
byte prev_bright;
byte prev_random;

void setup()
{
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip).setDither(brightness_k < 255);

  Serial.begin(115200);
  pinMode(13, OUTPUT);
  pinMode(3, INPUT_PULLUP);
  lcd_change = 1;
  pinMode(2, OUTPUT);
  randomSeed(analogRead(A0) + analogRead(A1) - analogRead(A2));
  start_timer = millis();
}
void loop()
{
  FastLED.setBrightness(brightness_k);

  if (millis() - button_debounce_timer >= 80 && digitalRead(3) == 0 && flag != 1)
  {
    button_debounce_timer = millis();
    if (change < 5)
    {
      change++;
      lcd_change = 1;
    }
    else
    {
      change = 1;
      lcd_change = 1;
    }
    //change = !change;
    flag = 1;
    //lcd_change = 1;
  }
  Serial.println(brightness_k);
  if (digitalRead(3) == 1 && flag == 1)
  {
    flag = 0;
  }
  if (millis() - change_timer >= 20000)
  {
    change_timer = millis();
    change++;
    if (change > 6)
    {
      change = 1;
      color_flag = true;
    }
    if (change == 2 && !random(2))
    {
      change++;
    }
    /*
    change = 6;
    if (change == 6)
    {
      for (int id = 0; id < NUM_LEDS; id++)
      {
        leds[id] = CRGB(0, 0, 0);
      }
      draw_new_snow = true;
      leds[27] = CHSV(255, 0, 255);
      snowflakes_timer = millis();
    }
    FastLED.show();
    */
    if (change == 7)
    {
      balls_what_to_do = 0;
      prev_bright = brightness_k;
      random_balls_color_arr[NUM_LEDS] = 1;
    }
  }

  if (change == 1)
  {
    if (color_flag)
    {
      color_flag = false;
      byte rnd_color = random(0, 4);
      if (rnd_color == 0)
      {
        hue_start_int = 0;
      }
      else if (rnd_color == 1)
      {
        hue_start_int = 65;
      }
      else if (rnd_color == 2)
      {
        hue_start_int = 115;
      }
      else if (rnd_color == 3)
      {
        hue_start_int = 210;
      }
    }
    fireTick();
    //running_lights();
  }
  else if (change == 2)
  {
    rainbow();
  }
  else if (change == 3)
  {
    bounce_ball_effect();
  }
  else if (change == 4)
  {
    christmas_lights();
  }
  else if (change == 5)
  {
    running_lights();
  }
  else if (change == 6)
  {
    random_balls();
  }
  else if (change == 7)
  {
    //snowflakes();
  }
  //Serial.println("loop");

  //brightness_k=255;
  //smooth_k_int=0.08;
  //hue_start_int=2;
}

void fireTick()
{
  static uint32_t prevTime, prevTime2;

  if (millis() - prevTime > 100)
  {
    prevTime = millis();
    for (int i = 0; i < ZONE_AMOUNT; i++)
    {
      zoneRndValues[i] = random(0, 10);
    }
  }

  if (millis() - prevTime2 > 20)
  {
    prevTime2 = millis();
    for (int i = 0; i < ZONE_AMOUNT; i++)
    {
      zoneValues[i] = (float)zoneValues[i] * (1 - SMOOTH_K) + (float)zoneRndValues[i] * 10 * SMOOTH_K;
      leds[i] = getFireColor(zoneValues[i]);
    }
    FastLED.show();
  }
}
/*
void rainbow()
{
  static byte counter = 0;
  counter += smooth_rainbow;

  // Iterate over each LED in the strip
  for (int i = 0; i < NUM_LEDS; i++)
  {
    // Set the LED color to the current hue value
    leds[i] = CHSV(counter + (i * 255 / NUM_LEDS), 255, 255);
  }

  FastLED.show();
}
*/

void fade_effect()
{
  static byte color = 0;
  color = fade_color;

  static bool shift;

  byte minuend;
  byte smooth_correct;
  if (smooth_fade >= 30)
  {
    minuend = 3;
    smooth_correct = 5;
  }
  else
  {
    minuend = 5;
    smooth_correct = smooth_fade;
  }

  if (millis() - fade_timer >= smooth_correct && shift == false)
  {
    fade_timer = millis();
    if (fade_brightness <= max_bright - minuend)
    {
      fade_brightness += minuend;
    }
    else
    {
      shift = true;
    }
  }
  else if (millis() - fade_timer >= smooth_correct && shift == true)
  {
    fade_timer = millis();
    if (fade_brightness >= min_bright + minuend)
    {
      fade_brightness -= minuend;
    }
    else
    {
      shift = false;
    }
  }
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV(color, 255, 255);
  }

  FastLED.show();
}

void fill_effect()
{
  static byte color = 0;
  color = fill_color;
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV(color, 255, 255);
  }

  FastLED.show();
}

void running_lights()
{
  byte comet_brightness_arr[] = {255, 255, 255, 255, 255, 220, 220, 220, 220, 220, 185, 185, 185, 185, 185, 150, 150, 150, 150, 150, 115, 115, 115, 115, 115, 80, 80, 80, 80, 80, 45, 45, 45, 45, 45, 10, 10, 10, 10, 10, 10};
  if (millis() - running_timer >= running_speed)
  {
    running_timer = millis();

    //CRGB color = CHSV(cur_color, 255, 255);
    if (millis() - random_timer >= running_freq)  //1310
    {
      leds[0] = CHSV(cur_color, 255, comet_brightness_arr[new_comet_id]);
      Serial.println(new_comet_id);
      new_comet_id++;
      if (new_comet_id == 41)
      {
        new_comet_id = 0;
        //if (random(0, 10) == 5)
        {
          cur_color += 50;
        }
        random_timer = millis();
      }
    }
    for (int i = NUM_LEDS - 1; i > 0; i--)
    {
      leds[i] = leds[i - 1];
    }
    FastLED.show();
  }
}

void rainbow()
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88(87, 220, 250);
  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;  //gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis;
  sLastMillis = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV(hue8, sat8, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS - 1) - pixelnumber;

    nblend(leds[pixelnumber], newcolor, 64);
  }
  FastLED.show();
}

void christmas_lights()
{
  if (millis() - christmas_lights_timer >= 500)
  {
    christmas_lights_timer = millis();

    for (int i = 0; i < NUM_LEDS; i++)
    {
      int randomEffect = random(3);
      byte white_random = random(2);

      if (randomEffect == 0 || (white_random == 0 && randomEffect != 1))
      {
        leds[i] = CRGB(255, 0, 0);
      }
      else if (randomEffect == 1 || (white_random == 0 && randomEffect != 0))
      {
        leds[i] = CRGB(0, 255, 0);
      }
      else if (white_random == 1)
      {
        leds[i] = CRGB(255, 255, 255);
      }
    }

    FastLED.show();
  }
}

void snowflakes()
{
  if (millis() - snowflakes_timer >= 200)
  {
    snowflakes_timer = millis();

    if (draw_new_snow)
    {
      draw_new_snow = false;
      leds[27] = CHSV(255, 0, 255);
      leds[snowflakes_pos[0]] = CHSV(255, 0, 0);
      snowflakes_pos[0] = 0;
      snowflakes_amount++;
    }
    byte random_snow;
    if (snowflakes_pos[0] == 0)
    {
      random_snow = random(1, 3);
    }
    else if (snowflakes_pos[0] == 1)
    {
      random_snow = random(2, 5);
    }
    else if (snowflakes_pos[0] == 2)
    {
      random_snow = random(1, 4);
    }
    else if (snowflakes_pos[0] == 3)
    {
      random_snow = random(3, 7);
    }
    else if (snowflakes_pos[0] == 4)
    {
      random_snow = random(2, 6);
    }
    else if (snowflakes_pos[0] == 5)
    {
      random_snow = random(1, 5);
    }
    else if (snowflakes_pos[0] == 6)
    {
      random_snow = random(2, 5);
    }
    else if (snowflakes_pos[0] == 7)
    {
      random_snow = random(2, 5);
    }

    leds[snowflakes_pos[0] + random_snow] = leds[snowflakes_pos[0]];
    leds[snowflakes_pos[0]] = CHSV(255, 50, 0);
    snowflakes_pos[0] = snowflakes_pos[0] + random_snow;
    FastLED.show();
  }
}

void bounce_ball_effect()
{
  if (millis() - moving_ball_timer >= 50)
  {
    moving_ball_timer = millis();
    for (int id = 0; id < NUM_LEDS; id++)
    {
      leds[id] = CRGB(0, 0, 0);
    }

    leds[ball_pos] = CHSV(ball_hue, 255, 255);

    ball_pos += ball_dir;

    if (ball_pos == 0 || ball_pos == NUM_LEDS - 1)
    {
      ball_dir = -ball_dir;
      ball_hue += 20;
    }
  }
  FastLED.show();
}

void random_balls()
{
  if (millis() - random_balls_timer >= 50)
  {
    random_balls_timer = millis();
    for (int led = 0; led < NUM_LEDS; led++)
    {
      byte random_color;
      byte random_hue;
      if (random_balls_color_arr[NUM_LEDS] != 3)
      {
        random_color = random(5);
        if (random_color == prev_random)
        {
          random_color = random(5);
        }
        switch (random_color)
        {
          case 0:
            random_hue = 7;
            break;
          case 1:
            random_hue = 20;
            break;
          case 2:
            random_hue = 150;
            break;
          case 3:
            random_hue = 0;
            break;
          case 4:
            random_hue = 60;
            break;
        }
        prev_random = random_color;
      }
      else
      {
        random_hue = random_balls_color_arr[led];
      }

      random_hue += random(1, 10);
      random_balls_color_arr[led] = random_hue;

      leds[led] = CHSV(random_hue, 255, 255);
    }
    random_balls_color_arr[NUM_LEDS] = 3;
    FastLED.show();
  }
}

void Ukraine()
{
  for (int i = 0; i < 36; i++)
  {
    leds[i] = CRGB(0, 0, 255);
    //leds[i+1] = CRGB(255,120,0);
  }
  for (int i = 37; i < 73; i++)
  {
    //leds[i] = CRGB(0,0,255);
    leds[i] = CRGB(255, 120, 0);
  }
  FastLED.show();
}

CRGB getFireColor(int val)
{
  return CHSV(HUE_START + map(val, 20, 60, 0, HUE_GAP),  // H
      constrain(map(val, 20, 60, MIN_SAT, MAX_SAT), 0, 255),  // S
      constrain(map(val, 20, 60, MIN_BRIGHT, MAX_BRIGHT), 0, 255)  // V
  );
}
