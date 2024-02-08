# O projektu 🚗
Specifikacija projektnog zadatka od strane project owner-a (uopšteni zahtjevi):

- autonomno vozilo
- upravljačka ploča (MCU: PIC24FJ64GAxx)
- lidar
- aplikacija

Cilj projektnog zadatka jeste realizacija autonomnog vozila koje ima mogućnost kretanja kroz prostoriju i mapiranje područja oko njega. Vozilo se sastoji od šasije sa 4 točka (2 slobodna točka i 2 točka pogonjena na DC motor) kojima upravlja upravljačka ploča bazirana na mikrokontroleru PIC24FJ64GA.

Kao ključni element same ploče, koristimo mikrokontroler iz familije PIC24FJ64GA za upravljanje i kontrolu sistema. Za komunikaciju sa već postojećom pločom koristimo I2C, a za komunikaciju sa drugim periferijama koristimo UART i SPI. Jedan od glavnih zahtjeva korisnika jeste da se ostvari bežična komunikacija i daljinsko upravljanje samog vozila, za šta je neophodno koristiti WiFi modul koji povezujemo putem UART interfejsa sa mikrokontrolerom. Mapiranje prostora i kontrola kretanja se vrši uz pomoć Lidara. Korisnik može kontrolisati kretanje vozila pomoću korisničke aplikacije i njenog GUI-a dok prati pogled na prostor u realnom vremenu. Na kraju, neophodno je razviti PC aolikaciju za prikaz reljefa oko samog vozila. Minimalni uslovi koji moraju biti ispunjeni su mogućnost pokretanja i zaustavljanja vozila.
Nakon prvog sastanka sa project owner-om, dobili smo detaljnije specifikacije samog projektnog zadatka koje podrazumijevaju sljedeće:

- PCB pločica može da bude jednoslojna ili dvoslojna
- prilikom izrade PCB ploče koristimo SMD komponente, gdje je to moguće
- minimalna širina veze prilikom rutiranja iznosi 0.254 mm
- nije potrebno projektovati dodatno napajanje, dovoljno je koristiti već postojeće
- budžet za narudzbu i kupovinu komponenata ne bi trebalo da prelazi 30 dolara

Početna šema, neophodna za izradu i projektovanje štampane ploče prikazana je na slici ispod, a detaljnije objašnjenje same šeme i načina spajanja iste nalazi se na Wiki stranici.

![image](https://github.com/codeandrelax/2DSpaceMapping_A/assets/127949851/6d128b33-2866-4c8a-ae0e-71f599b6bc59)


Nakon projektovane šema, kao rezultat dobijamo PCB i rutiranu pločicu, prikzanu na slici ispod :

![image](https://github.com/codeandrelax/2DSpaceMapping_A/assets/127949851/8b781960-7fe1-4c56-adbb-dc19849625e8)


Dostavljamo i 3D model PCB-a :

![image](https://github.com/codeandrelax/2DSpaceMapping_A/assets/127949851/cd37e0d4-cbff-4da7-91c0-ac58e72083cc)
