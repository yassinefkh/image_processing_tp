# TPs - Introduction à l'analyse d'images

## Enseignant : Pr. Camille KURTZ

Ce dépôt contient les fichiers des **TP4 et TP7** du cours d'Analyse d'Images.

### **TP4 - Segmentation d'Images**
Ce TP porte sur différentes méthodes de segmentation d'images.

#### **Fichiers inclus :**
- `Makefile` : Fichier de compilation.
- `connected_components.cpp` : Implémentation de l'étiquetage des composantes connexes.
- `otsu.cpp` : Algorithme de seuillage d'Otsu.
- `quadtreenode.cpp` : Segmentation par Split and Merge.
- `region_growing.cpp` : Segmentation par croissance de région (region growing).

---

### **TP7 - Edge & Keypoint Detection**
Ce TP porte sur quelques algorithmes de détection de points d'intérêt dans les images, ainsi que le calcul de la magnitude de gradient.

#### **Fichiers inclus :**
- `Makefile` : Fichier de compilation.
- `harris.cpp` : Détecteur de Harris pour la détection de coins.
- `fast.cpp` : Détecteur de points clés FAST.
- `hough.cpp` : Détection de lignes/cerles avec la transformée de Hough.
- `gradient.cpp` :Calcul de la magnitude du gradient.

---

### **Compilation et Exécution**
Pour compiler tous les fichiers, exécutez :
```sh
make && ./exec
```

En changeant dans le Makefile le fichier SRCS .cpp 
