# O projektu
<p align="justify">
<Specifikacija projektnog zadaatka od strane project owner-a>
Cilj projektnog zadatka jeste realizacija autonomnog vozila koje ima mogućnost kretanja kroz prostoriju i mapiranje područja oko njega. Vozilo se sastoji od šasije sa 4 točka (2 slobodna točka i 2 točka pogonjena na DC motor) kojima upravlja upravljačka ploča bazirana na mikrokontroleru PIC24FJ64GA. 
</p>
Kao ključni element same ploče, koristimo mikrokontroler iz familije PIC24FJ64GA za upravljanje i kontrolu sistema. Za komunikaciju sa već postojećom pločom koristimo I2C, a za komunikaciju sa drugim periferijama koristimo UART i SPI.
Jedan od glavnih zahtjeva korisnika jeste da se ostvari bežična komunikacija i daljinsko upravljanje samog vozila za šta nam je neophodno koristiti WiFi modul koji povezujemo putem UART interfejsa sa mikrokontrolerom.
![image](https://github.com/codeandrelax/2DSpaceMapping_A/assets/127949851/8e4bbf51-5709-415c-8d27-cc377c6c054b)
![image](https://github.com/codeandrelax/2DSpaceMapping_A/assets/127949851/0eff38dd-d52b-4730-8ba2-136af74acea5)

Mapiranje prostora i kontrola kretanja se vrši uz pomoć Lidara. Korisnik može kontrolisati kretanje vozila pomoću korisničke aplikacije i njenog GUI-a dok prati pogled na prostor u realnom vremenu.
