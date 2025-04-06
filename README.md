# Sándor János, Neptunkód: U03GDK, Miskolci Egyetem, Számítógépes Grafika

## Pohár vízzel – 3D grafikus feladat (kibővített leírás)

- project:
	A felhasználó egy óriási pohár belsejébe kerül egy 3D-s térben.
	A kamera szabadon mozgatható egérrel és billentyűzettel.

- scenes:
	- 1. kezdeti_allapot:
		A pohár tele van vízzel.
		A tér sötét, csak a falak és a víz szintje látszik.
	
	- 2. kamera_leereszkedese:
		A fény fokozatosan erősödik.
		A pohár falai lassan áttetszővé válnak.
	
	- 3. alj_elérése:
		A pohár feldől.
		A víz kifolyik egy animációval.
		A pohár teljesen kiürül.
	
	- 4. utojelenet:
		Egy animált kocka görbén végighalad a pohár külső falán.
		A poharat visszaállítja az eredeti pozícióba (víz nélkül).

- interakciók:
	kamera_iranyitas: "WASD + egér"

	fenyero_szabalyozas: "+" és "-" billentyűk

	segítség: "F1 gombbal megjelenő használati útmutató"

	víz_szimulálása:
		Egyszerű átlátszó kék objektum (pl. félhenger vagy doboz).
		Vízszint animáció: objektum magasságának csökkentése.
		Kifolyás: skálázás + animált textúra/mesh.

	víz_felett_alatt_különbség:
		Víz alatt: kékes filter, gyengébb fény, torzítás (shader vagy effekt).
		Víz felett: normál fényerő, tiszta látvány.
		Megvalósítás: kamera pozícióhoz kötött shader-váltás.

	kamera_víz_interakció:
		Nincs fizikai interakció.
		A kamera csak vizuális effekteket vált ki.
		Extra: lefelé mozgásnál szűrt látvány (blur, színváltás stb.)

	pohár_környezete:
		Egyszerű tér (végtelen padló + világos háttér).
		Vagy stílusos letisztult 3D környezet (pl. üvegterem).
		Fókusz: a pohár belső világa.
