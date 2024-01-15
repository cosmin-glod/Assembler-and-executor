
# ASAMBLOR ȘI EXECUTOR ARHITECTURA RISC
  Proiect pentru Arhitectura Sistemelor de Calcul (Arhitectura Calculatoarelor), Facultatea de Matematică și Informatică, Secția Informatică, Universiatea din București, anul 2023-2024.
  Proiect realizat de:



## Echipa Albăstrelele, seria 15, grupa 151

- [@Andu9](https://github.com/Andu9) Nita Alexandru
- [@cosmin371](https://github.com/cosmin371) Glod Cosmin-Stefan
- [@danielbirsan](https://github.com/danielbirsan) Birsan Gheorghe Daniel

## Despre
-Asamblorul codifica instructiunile si le scrie intr-un fisier binar, urmand sa fie executate de catre executor. In acest repo se gasesc codificarile facute pentru instructiuni, registri si functiile din C++.


##  Instalare si requirements

- Compilator C++: (GCC, Clang sau Microsoft Visual C++).
- Editor de text sau IDE: (Code::Blocks, Visual Studio Code).
- Sistem de operare: linux/windows

+Binary viewer (ex: https://binary-viewer.en.softonic.com/download)


## Testare

Pentru testare s-au folosit următoarele fișiere sursă:

https://marz.utk.edu/my-courses/cosc230/book/example-risc-v-assembly-programs/


## Rularea codului
- Asamblor:
- -----LINUX-----
-- se descarca codul sursa
-- se ruleaza comanda "g++ asamblor.cpp -o asamblor" in terminal in folderul cu sursa
-- in fisierul "input.in" se pune programul in assembly risc-v ce urmeaza a fi asamblat
- se ruleaza comanda "./asamblor"
- in fisierul "executabil.bin" se afla binarul din urma executarii
- Executor:
To do


## Cerința Proiectului
 Crearea unui standard de cod mașină și apoi un executor pentru fișierele binare generate
- https://cs.unibuc.ro/~crusu/asc/Arhitectura%20Sistemelor%20de%20Calcul%20(ASC)%20-%20Proiect%200x00%202023.pdf


## License

[MIT](https://choosealicense.com/licenses/mit/)

