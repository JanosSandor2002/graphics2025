# Pohár vízzel – 3D grafikus feladat

**Készítette:**  
Sándor János  
Neptun kód: U03GDK  
Miskolci Egyetem, Számítógépes Grafika tantárgy

---

## Projekt leírás

A felhasználó egy óriási, üres pohár belsejébe kerül egy 3D-s térben.
A pohár teljes terét sűrű, vastag köd tölti ki, ami megnehezíti a látást és különleges atmoszférát teremt.
A kamera szabadon mozgatható egérrel és billentyűzettel.
A jelenetek és interakciók folyamatosan váltakoznak az animáció során.

---

### Jelenetek (scenes)

- **kezdeti_allapot**  
  - A pohár tele van vízzel.  
  - A tér sötét, csak a falak és a víz szintje látszik.

- **kamera_leereszkedese**  
  - A fény fokozatosan erősödik.  
  - A pohár falai lassan áttetszővé válnak (alpha blending).

- **alj_elerese**  
  - A pohár feldől.  
  - A víz kifolyik animációval (mesh skálázás + textúra animálás).  
  - A pohár teljesen kiürül.

- **utojelenet**  
  - Visszaállít minden modellt a helyére.

---

### Interakciók

- **Kamera irányítás:**  
  `WASD` + egér mozgatás (szabad nézet és irányítás)

  `SPACE` + `C` fel és le mozgás

- **Fényerő szabályozás:**  
  `+` és `-` billentyűkkel

- **Segítség (útmutató):**  
  `F1` gombbal megjelenő súgó

---

### Víz szimulálása

- Egyszerű átlátszó kék objektum (pl. félhenger vagy doboz mesh).
- **Vízszint animáció:**  
  - Az objektum magassága fokozatosan csökken (skálázás).
- **Kifolyás animáció:**  
  - Mesh skálázása + animált textúra (UV scroll) + vertex shader hullámzás.

---

### Víz alatti és feletti különbség (shader-ekkel)

- **Víz alatt:**  
  - Kékes filter, gyengébb fényerő, torzítás effekt (GLSL shaderrel).  
  - Megvalósítás: kamera pozícióhoz kötött shader váltás.  

- **Víz felett:**  
  - Normál fényerő, tiszta látvány.

- **Technikai részletek:**  
  - OpenGL 2.0 (GLSL 1.10) shader-ek használata:  
    - `vertex shader` és `fragment shader` kézzel betöltve és lefordítva.  
  - Post-process effekt (víz alatti torzítás) FBO + shaderrel megoldva.  
  - Pohár áttetszőség: alpha blending (`glBlendFunc`) + fragment shader alpha csökkentéssel.

---

### Kamera és víz interakció

- Nincs fizikai interakció, csak vizuális effektek.  
- Lefelé mozgásnál szűrt látvány:  
  - Blur imitáció és színváltás GLSL shaderrel.

---

### Pohár környezete

- Egyszerű tér: végtelen padló + világos háttér.  
- Stílusos letisztult 3D környezet (pl. üvegterem).  
- Fókusz: a pohár belső világa.

---

## Shader-ek megvalósítása (OpenGL 2.0 + GLSL 1.10)

- Minden shader manuálisan lesz betöltve:  
  `glCreateShader`, `glShaderSource`, `glCompileShader`, stb.
- Nem használ modern GLU vagy GLUW bővítményeket.
- Víz alatti effekt shader:  
  - Képernyő torzítás (UV hullám), szín filter GLSL fragment shaderrel.
- Pohár áttetszőség: alpha blending + shader alpha változtatása.

---

