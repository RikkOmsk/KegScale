# KegScale
Keg Scale


# Железо
Потребуются 
  1. ESP8266        https://aliexpress.ru/item/4000160133215.html
  2. AM2302 DHT22   https://aliexpress.ru/item/32759901711.html
  3. HX711          https://aliexpress.ru/item/1005002176327047.html


# Схема подключения
/scheme/main.jpg


# Прошивка под Arduino
/ESP8266/ESP8266_keg_v02.ino
Необходимо заменить все что под ***
 - auth key blynk
 - ssid wifi
 - pass wifi

# 3D модель для печати
/3dModel/print_01.stl - первая модель площадки. Печать с поддержками 40% заполнения
/3dModel/flexTop v1.stl - первая модель площадки. Флекс резиновое кольцо сверху

/3dModel/print_02.stl - вторая модель площадки
/3dModel/log02_im1.png - список изменений
/3dModel/log02_im2.png - список изменений
/3dModel/log02_im3.png - список изменений

Список изменений v02: 
0. Печататься деталь будет вверх ногами.
1. Сделал паз под провода. Как оказалось, тензодатчики сложно установить без этого паза.
2. Убрал держатели для тензодатчиков. Паз ровно под размер датчиков и встает туда с усилием. Держатели мешали установке детчиков
3. Под NodeMCU дал еще 2 мм прострнаства. Сам ESP находится под смещенем на NodeMCU.
4. Увеличил размеры гнезда под microUSB. Не все штекера подошли.
5. Немного уменьшил вылеты площадки
6. Убрал паз под резиновое кольцо и поменяло форму кольца. Кольцо будет печататься в два слоя сразу же поверх площадки, что сделает изделие аккуратнее, резину тоньше и шире.
7. Сделал более пологий скос, чтобы при печати вверх ногами не потребовались поддержки.


# Калибровка 
Во время отладки необходимо настроить два поля:
int keg1_calibration_factor = -21020;   // Калибровочная значение, индивидуальное для каждой итоговой площадки
int keg1_offset_factor = -64200;        // 0 вес для площадки (сюда вставляем значение, который прилетает в keg1_raw при своем пустом весе)
1. Эти значения будут уникальны для каждого собранно устройства. Начинаем с keg1_offset_factor = в него устанавливаем значение, которое прийдет в keg1_raw при запуске устройства.
2. Далее нужно разместить заранее известный точный вес (например гирю либо бутылку с водой). К примеру вес гири 5кг. Далее мы смотрим на значение в консоли веса, который отображает ваше устройство. Если значение меньше либо больше, то необходимо поправить keg1_calibration_factor в меньшую либо большую сторону и щалитьпрошивку заново. Повторяйте до тех пор пока ваша площадка не будет отображать ровно 5 кг как и гиря.

# Настойка blynk
(будет позже)




# Бонус
washKeg_0.2 - Мойка для кег.
- washKeg_v0.2_top2.stl - 1 шт на кег 10л, вторую накручиваем если кег на 19.
- washKeg_v0.2_top1.stl 2шт.
- washKeg_v0.2_line1.stl 1шт. 
- washKeg_v0.2_line2.stl 2шт.
- washKeg_v0.2_dash.stl 1шт.
- washKeg_v0.2_stat.stl 4шт.
