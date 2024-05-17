# O projektu 🚗
<p align="justify">
Specifikacija projektnog zadatka od strane project owner-a (uopšteni zahtjevi) :
  
- autonomno vozilo
- upravljačka ploča (MCU: PIC24FJ64GAxx)
- lidar
- aplikacija
  
Cilj projektnog zadatka jeste realizacija autonomnog vozila koje ima mogućnost kretanja kroz prostoriju i mapiranje područja oko njega. Vozilo se sastoji od šasije sa 4 točka (2 slobodna točka i 2 točka pogonjena na DC motor) kojima upravlja upravljačka ploča bazirana na mikrokontroleru PIC24FJ64GA. 
</p>
Kao ključni element same ploče, koristimo mikrokontroler iz familije PIC24FJ64GA za upravljanje i kontrolu sistema. Za komunikaciju sa već postojećom pločom koristimo I2C, a za komunikaciju sa drugim periferijama koristimo UART i SPI.
Jedan od glavnih zahtjeva korisnika jeste da se ostvari bežična komunikacija i daljinsko upravljanje samog vozila, za šta je neophodno koristiti WiFi modul koji povezujemo putem UART interfejsa sa mikrokontrolerom.
Mapiranje prostora i kontrola kretanja se vrši uz pomoć Lidara. Korisnik može kontrolisati kretanje vozila pomoću korisničke aplikacije i njenog GUI-a dok prati pogled na prostor u realnom vremenu.
Na kraju, neophodno je razviti PC aolikaciju za prikaz reljefa oko samog vozila. Minimalni uslovi koji moraju biti ispunjeni su mogućnost pokretanja i zaustavljanja vozila.



Nakon prvog sastanka sa project owner-om, dobili smo detaljnije specifikacije samog projektnog zadatka koje podrazumijevaju sljedeće:
- PCB pločica može da bude jednoslojna ili dvoslojna
- prilikom izrade PCB ploče koristimo SMD komponente, gdje je to moguće
- minimalna širina veze prilikom rutiranja iznosi 0.254 mm
- nije potrebno projektovati dodatno napajanje,  dovoljno je koristiti već postojeće
- budžet za narudzbu i kupovinu komponenata ne bi trebalo da prelazi 30 dolara

Početna šema, neophodna za izradu i projektovanje štampane ploče prikazana je na slici ispod, a detaljnije objašnjenje same šeme i načina spajanja iste nalazi se na Wiki stranici.


![image](https://github.com/codeandrelax/2DSpaceMapping_A/assets/127949851/2f7372e2-dae2-4764-a45e-1873a220e018)

Nakon projektovane šema, kao rezultat dobijamo PCB i rutiranu pločicu, prikzanu na slici ispod:

![image](https://github.com/codeandrelax/2DSpaceMapping_A/assets/127949851/6fed7947-294c-46de-b49b-b1b22bd3d8e7)

Dostavljamo i 3D model PCB-a :

![image](https://github.com/codeandrelax/2DSpaceMapping_A/assets/127949851/f9999e05-9234-46d4-8bc6-666b6d9066e5)

Na slici ispod se nalazi sklopljeno autonomno vozilo sa pratećim komponentama neophodnim za pravilan rad samog vozila.

![size](https://github.com/codeandrelax/2DSpaceMapping_A/assets/127949851/459205ed-d0a2-4750-8809-29c22ccc8fbd)

Demonstracija skeniranja prostora IR senzorom i slanje podataka na aplikaciju za prikaz prostora data je na sledećem snimku.

![radar-ezgif com-optimize](https://github.com/codeandrelax/2DSpaceMapping_A/assets/117661784/7efa6d1f-04dd-4f3f-a125-2b5ae2cb0a6d)

Demonstracija kretanja vozila pored prepreka data je na sledećem snimku.

![drive-ezgif com-optimize](https://github.com/codeandrelax/2DSpaceMapping_A/assets/117661784/4cef959a-57d1-4617-9e94-70d19f6dd524)

