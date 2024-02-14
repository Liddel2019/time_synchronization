#include <Arduino.h>
#include <Ds1302.h>

const int PIN_RST = 5;  // Reset
const int PIN_CLK = 18; // Clock
const int PIN_DAT = 19; // Data

Ds1302 rtc(PIN_RST, PIN_CLK, PIN_DAT);

const char* WeekDays[] = {
    "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
};

unsigned long previousMillis = 0;
const long interval = 1000; // Интервал между обновлениями (1 секунда)

// Переменные для компенсации времени
int compensationHour = 0;
int compensationMinute = 0;
int compensationSecond = 2;

void setup() {
    Serial.begin(115200);
    rtc.init();
    Serial.println("DS1302 RTC module initialized");
}

void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        Ds1302::DateTime now;
        rtc.getDateTime(&now);
        
        // Компенсация времени
        int hour = now.hour + compensationHour;
        int minute = now.minute + compensationMinute;
        int second = now.second + compensationSecond;

        // Корректировка переполнения или недостатка
        if (second >= 60) {
            second -= 60;
            minute += 1;
        } else if (second < 0) {
            second += 60;
            minute -= 1;
        }
        if (minute >= 60) {
            minute -= 60;
            hour += 1;
        } else if (minute < 0) {
            minute += 60;
            hour -= 1;
        }
        if (hour >= 24) hour -= 24;
        else if (hour < 0) hour += 24;

        Serial.print("20");
        Serial.print(now.year);
        Serial.print('-');
        if (now.month < 10) Serial.print('0');
        Serial.print(now.month);
        Serial.print('-');
        if (now.day < 10) Serial.print('0');
        Serial.print(now.day);
        Serial.print(' ');
        Serial.print(WeekDays[now.dow - 1]);
        Serial.print(' ');
        if (hour < 10) Serial.print('0');
        Serial.print(hour);
        Serial.print(':');
        if (minute < 10) Serial.print('0');
        Serial.print(minute);
        Serial.print(':');
        if (second < 10) Serial.print('0');
        Serial.print(second);
        Serial.println();
    }

    // Асинхронное чтение данных с последовательного порта
    if (Serial.available() > 0) {
        String timeString = Serial.readStringUntil('\n');
        // Здесь можно добавить обработку строковых данных для настройки компенсации
        // Например: "setCompensation 1 15 30" для установки компенсации на +1 час 15 минут 30 секунд
    }
}
