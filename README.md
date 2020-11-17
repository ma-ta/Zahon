# BýloMetr

> Wi-Fi monitorovací stanice pro záhon či květináč s vestavěným webovým serverem

## Monitorované veličiny
- teplota
- úroveň světla
- vlhkost půdy
- srážky

![Fotodokumentace](/fotodokumentace/001.jpg)

### Etapa projektu
- [x] 1. zprovoznit prototyp
- [ ] 2. ukládání dat
- [ ] 3. optimalizovat kód a fyzické zapojení
- [ ] 4. zpřesnit měření (např. více čidel pro průměrování)
- [ ] 5. napájení z akumulátoru
- [ ] 6. miniaturizovaná case
- [ ] 7. software na zpracování dat
- [ ] 8. dokumentace, návody

### Hlavní HW komponenty
- MCU ATMega328P (resp. Arduino Uno),
- Wi-Fi modul ESP8266-01,
- LCD displej z Nokie 5110,
- senzory:
  - fotorezistor,
  - teplotní čidlo DS18B20,
  - detektor srážek,
  - čidlo pro měření vlhkosti půdy.
- zdroj, rezistory, (převodník napěťových úrovní logických hodnot TTL na 3,3 V), nepájivé kontaktní pole, kabely.
- (USB-Serial převodník na programování Wi-Fi modulu ESP8266)
