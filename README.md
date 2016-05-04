# BýloMetr (IoT)

> Digitální monitorovací stanice pro záhon či květináč (teplota, světlo, vlhkost půdy, srážky)

![Fotodokumentace] (/fotodokumentace/001.jpg)

### Etapa projektu
- [x] 1. zprovoznit prototyp
- [ ] 2. optimalizovat kód a fyzické zapojení
- [ ] 3. doplnit více čidel pro výpočet průměru
- [ ] 4. napájení z akumulátoru
- [ ] 5. miniaturizovaná case
- [ ] 6. software na zpracování dat
- [ ] 7. dokumentace, návody

###Hlavní HW komponenty
- MCU ATMega328P (resp. Arduino Uno),
- Wi-Fi modul ESP8266-01,
- LCD displej z Nokie 5110,
- senzory:
  - fotorezistor,
  - teplotní čidlo DS18B20,
  - detektor srážek,
  - čidlo pro měření vlhkosti půdy.
- zdroj, rezistory, (převodník napětí TTL na 3,3 V), nepájivé kontaktní pole, kabely.
- (USB-Serial převodník na programování ESP8266)
