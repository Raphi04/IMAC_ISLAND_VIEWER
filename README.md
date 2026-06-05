# IMAC_ISLAND_VIEWER

> Le projet est réalisé sur Windows

Raphael CADETE | Julien LEEDER | Yanis WONG

## Bruit fractal

Utilisation des sites mis à disposition pour la compréhension du fonctionnement du bruit et de l'utilité de chaque variable.
https://thebookofshaders.com/13/?lan=fr
https://iquilezles.org/articles/fbm/

## Génération de heightmap et couleurs

Random de Jules

## Distribution de points par Poisson disk sampling

Pour réaliser le Poisson disk sampling, nous avons suivi le [tutoriel de Sebastian Lague](https://www.youtube.com/watch?v=7WcmyxyFO7o) qui code cette algorithme sur un projet Unity. Il était alors nécessaire de traduire son code C# en C++.

## Placement des objets sur le terrain

## Paramètres visuels

Afin de tester différentes configurations, vous pouvez modifier certaines variables depuis l'interface utilisateur.

### Aléatoire

Vous pouvez modifier les paramètres suivants de manière aléatoire :

- **Position aléatoire** : modifie le placement des objets sur l'île

- **Génération aléatoire de l'île** : modifie les variables de génération de l'île

- **Environnement aléatoire** : selectionne une couleur d'environnement

- **Limites de hauteur aléatoire** : modifie les variables de limites de hauteur des objets

### Environnements

Vous pouvez séléctionner un des environnements suivants : Plaine, Arctique, Desert, Magma, The End, Mesa.

### Poisson disk sampling

Vous pouvez modifier les paramètres suivants pour le Poisson disk sampling :

- **Nombre d'essaie avant rejet** : nombre de fois que l'algorithme va essayer de trouver un point valide autour d'un point déjà placé

- **Rayon minimal** : distance minimal entre deux points

- **Nombre de points maximum** : nombre de points maximum à afficher sur l'ile

### Objets

Vous pouvez modifier les paramètres suivants pour les objets :

- **Échelle de l'objet** : modifie la taille de l'objet

- **Hauteur minimale** : modifie la limite de hauteur minimale

- **Hauteur maximale** : modifie la limite de hauteur maximale

### Masque

Vous pouvez modifier les paramètres suivants pour le masque :

- **Esperance** : modifie l'esperance de la fonctione gaussienne

- **Ecart type** : modifie l'ecart type de la fonctione gaussienne

## Difficultés rencontrées et solutions

### Bruit fractal

La difficulté principale que j'ai rencontré, c'est la compréhension de comment utiliser chaque variable pour faire le bruit, car même si je comprenais ce qu'elle devait faire, je n'arriver pas à comprendre comment le retranscrire en code.

### Heightmap et couleurs

### Poisson disk sampling

Le plus dure dans le Poisson disk sampling etait de réussie à traduire le code C# en code C++, mais aussi de réussir à comprendre ce que faisait l'algorithme en détail (comme par exemple le fait de stocker l'index d'un point dans la grille).

## Captures d'écran comparatives

Voici des images comparatives de nos changements.

### Bruit fractal

### Heightmap et couleurs

### Poisson disk sampling

## Retrospection

Voici une courte retrospection de notre travail.

### Qu'est ce qui a bien fonctionné :

Puisque les exercices étaient indépendants, on a bien pu se répartir les tâches sans que l'avancée des uns influe celle des autres.

### Problème rencontrés, solutions et ce qu'on aurait fait différement :

Pour le Poisson disk sampling, nous avons initialement codé une version qui n'utilise pas la grille, mais qui était donc peu performante. On a alors étudié la [vidéo de Sebastian Lague](https://www.youtube.com/watch?v=7WcmyxyFO7o) qui, lui, utilise la grille et on l'a réadapté à notre projet.

### Ce qu'on peut améliorer :

On pourrait optimiser les performances du programme en faisant en sorte que quand on modifie les valeurs des sliders dans l'interface utilisateur, on ait un délay avant de regénérer l'île. L'interface utilisateur pourrait être revu pour la rendre plus lisible (avec des couleurs, un allignement horizontal, etc.). La fenêtre de visualisation pourrait aussi s'ouvrir directement en plein écran.

### Répartition du travail :

Voici la répartition du travail dans notre groupe :

- **Yanis WONG** : Bruit fractal et interface utilisateur

- **Raphael CADETE** : Poisson disk sampling, README.md, relecture, corrections et interface utilisateur

- **Julien LEEDER** : Heightmap et couleurs, limite de hauteur, aléatoire et interface utilisateur
