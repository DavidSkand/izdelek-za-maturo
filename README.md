# Projekt: Zaznavanje AprilTagov s kamero

## Opis

Ta projekt uporablja računalniški vid za zaznavanje **AprilTagov** z uporabo kamere. Ko kamera zazna AprilTag, prikazuje ID taga ter koordinate njegovega središča na sliki. Koda uporablja knjižnici **OpenCV** za zajemanje slike in **pyapriltags** za zaznavanje AprilTagov.

## Povezane tehnologije

- **OpenCV**: Knjižnica za obdelavo slik in računalniški vid.
- **pyapriltags**: Knjižnica za zaznavanje **AprilTagov** v slikah.
- **Python**: Glavni programski jezik.

## Funkcionalnost

- **Zajem slike**: Program zajema slike s kamere (kamera je nastavljena na napravo 1).
- **Pretvorba v sivo sliko**: Slika se pretvori v sivinsko lestvico, kar omogoča lažje zaznavanje tagov.
- **Zaznavanje AprilTagov**: Program zaznava **AprilTags** na sliki in določi njihova središča.
- **Prikaz koordinate in ID taga**: Na sliki so označena središča AprilTagov, prikazane so njihove koordinate in ID številka.
- **Prikaz FPS**: Na sliki je prikazan trenutni okvir na sekundo (FPS), kar omogoča spremljanje hitrosti obdelave slike.
- **Prekinitev**: Program omogoča izhod iz programa, ko uporabnik pritisne tipko `q`.

## Postopek

1. **Inicializacija kamere**: Program preveri, ali je kamera pravilno odprta.
2. **Zajem podatkov**: Začne se zajemanje slik s kamere.
3. **Detekcija tagov**: Vsaka slika je obdelana za zaznavanje AprilTagov.
4. **Prikaz rezultatov**: Program nariše pravokotnik okoli zaznanega taga, prikaže njegov ID in koordinate na sliki.
5. **Kratek izračun FPS**: Izračuna se hitrost procesiranja slik v okvirih na sekundo (FPS).
6. **Zaključek**: Program omogoča izhod s pritiskom na tipko `q`.

## Navodila za zagon

1. Namestite potrebne knjižnice:
    ```bash
    pip install opencv-python pyapriltags
    ```

2. Zaženite skripto:
    ```bash
    python apriltag_detection.py
    ```

3. Pritisnite `q`, da zaprete okno kamere.

## Uporabljeni deli
-**MG90S**
-**Laser diode 5mW red 650nm 5V**



