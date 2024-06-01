---
jupytext:
  text_representation:
    extension: .md
    format_name: myst
    format_version: 0.13
    jupytext_version: 1.14.1
kernelspec:
  display_name: C++17
  language: C++17
  name: xcpp17
---

# Rapport de projet *Données libres*

*Les paragraphes en italique comme celui-ci vous donnent des
indications sur le contenu attendu; ils sont à supprimer une fois
votre rapport rédigé.*

*Ce rapport pourra servir de support pour votre présentation orale.*

*Soyez objectifs et factuels ! Vous ne serez pas évalué sur ce que vous
affirmez, mais sur la pertinence et la justesse de ce que vous
affirmez.*

+++

## Auteurs

- Longuepee, Lubin, lubin.longuepee@universite-paris-saclay.fr, LDD1 IM1
- Boucharé, Rémi, remi.bouchare@universite-paris-saclay.fr, LDD1 IM1

+++

## Résumé du travail effectué

- Exercice 1: Tout fonctionne correctement, sans trop de difficultés
- Exercice 2: Exercice réalisé et fonctionne, pas de difficulté particulière rencontrée
- Exercice 3: Tout fonctionne correctement, la seule difficulté était d'écrire le while car on voulait de base écrire while(fichier) sauf que cela rentrait une fois de trop dans la boucle
- Exercice 4: Exercice réalisé et fonctionne, j'ai eu un peu de mal à écrire la onction litTableau, notamment à cause d'une ligne en trop à la fin, qui n'exitais pas dans le fichier. La fonction
main m'a égalemment posé problème notamment à cause des multiple appel à selectLignes, ainsi, j'ai dû faire le choix de priviligier l'efficacité sur la lisibilité car j'avais beaucoup trop de variables (tableau obtenus par des appels à selectLignes ou colonne). J'ai égalemment dû faire attention aux nombres de boucles que j'implémentais, car le programme mettait beaucoup trop de temps à calculer, ainsi, j'ai dû optimiser ma fonction main, ce qui rend le code encore moins lisible.
- Exercice 5: Tout fonctionne correctement, pas de difficulté particulière
- Exercice 6: Exercice réalisé et fonctionne, pas de difficulté particulière rencontrée, excepté pour l'implentation de groupByInt, pour comprendre ce qu'elle renvoyait et pour l'ordre des deux boucles for
- Exercice 7: Tout fonctionne correctement, la seule difficulté était de bien comprendre comment utiliser la fonction getline
- Exercice 8: Tout fonctionne correctement, la seule difficulté était de bien comprendre que le dernier caractère de chaque ligne était un "\r" qu'il fallait enlever
- Exercice 9: Exercice réalisé et fonctionne, pas de difficulté particulière rencontrée
- Exercice 10: Exercice non réalisé
- Exercice 11: Exercice réalisé et fonctionne, pas de difficulté particulière rencontrée, hormis la réalisation à la main du fichier (assez longue) et le traitement de fichier (comprendre qu'il fallait convertir toutes les , en ; par exemple) 
- Exercice 12: Tout fonctionne correctement (j'ai l'impression). Pour cet exercice, on a voulu chercher à utiliser des éléments hors programmes puisque c'était un exerice "pour aller plus loin". On a donc voulu utiliser les dicionnaire avec la classe "map". La seule difficulté de l'exercice a été de comprendre comment ceux-ci fonctionnaient pour pouvoir les utiliser comme on le voulais
- Exercice 13: Exercice réalisé et fonctionne, pas de difficulté particulière rencontrée, fichier trouvé directement prêt à être utilisé 
- Exercice 14: Tout fonctionne correctement (même si les données ne sont pas très régulières, il en manque beaucoup). On a voulu pour cet exercice, calculer la moyenne de la qualité de l'air annuelle dans 2 stations de métro parisiennes. Pour cela, la seule difficulté a été de créer une fonction split, qui fractionnait la chaine de caractère correspondant à la date et l'heure du relevé, afin de ne garder que l'année.
- Exercice 15: Exercice non réalisé

+++

## Démonstration

*Soutenez ce que vous affirmez dans la section précédente au moyen des exemples ci-dessous.*

*Vous pouvez lancer des commades shell (terminal) depuis cette feuille en précédant la commande d'un point d'exclamation (mais cela ne permet pas l'ineteraction avec le terminal comme entrer une année ou un prénom). Les trois commandes ci-dessous lance la compilation de tous les exercices guidés puis l'ensemble des tests*

*Editez et complétez pour que cela corresponde à votre travail (par exemple en rajoutant la compilation et les tests qui correspondent aux exercices plus avancés). Indiquez aussi de quels programmes vous souhaitez nous faire la démonstration lors de la soutenance. Choisissez plutôt des exemples pertinents qui mettent en valeur votre
travail. Inutile de perdre du temps sur les premier niveaux si vous
avez fait les suivants plus compliqués (on se contentera des tests automatiques)*

```{code-cell}
! make clean
```

```{code-cell}
! make all
```

```{code-cell}
! make test
```

```{code-cell}
! ./voiture
```

```{code-cell}
! ./parapluie
    
```

```{code-cell}
! ./frequentation-metro-rer
```

```{code-cell}
! ./qualiteair
```

## Organisation du travail

Rémi : Je me suis réservé des créneaux allant de 2 heures miminum à 4 heures maximum par jour, les jours où cela était possible (Lundi, Mardi, Jeudi, Samedi, Dimanche), afin de ne pas commencer un exercice sans le finir, ni non plus de faire de trop longues sessions. J'ai ainsi consacré environ 24 heures au projet (rapport + commentaires compris), en dehors des TD ( donc environ 30 heures). Le problème étant que j'ai consacré environ 1/4 du temps à l'exercice 4, d'une part à cause du grand nombre d'appels à selectLignes et colonnes qui m'enmbrouillaient, et également du fait que je testais mon programme sur les noms du tableau test (de 4 lignes) qui n'existent pas dans le vrai fichier de 23 000 lignes...

Lubin: Pour ce projet, nous nous sommes organisés de telle sorte que chacun de nous deux avance sur un ou deux exercices d'une séance de TP sur l'autre, pour pouvoir surmonter les éventuelles difficultés ensemble lors des séances de TP consacrées au projet et pour que la personne du binôme qui n'a pas approfondi l'exerice de son côté puisse le comprendre. Le nombre d'heure consacrées au projet est similaire pour nous deux, et est d'une quinzaine d'heure, en plus des heures de TP consacrées au projet.

Nous nous sommes ainsi répartis le projet en exercices associés pour les premiers exercices, puis un exercice sur deux pour les derniers. Lubin a ainsi réalisé les exercices 1,3,5,7,8,12 et 14 et Rémi les exercices 2,4,6,9,11 et 13. 

Cette manière de travailler était optimisée pour nous puisque nous ne ressentions pas le besoin de travailler à deux sur les exercices simultanément étant donné qu'ils nous paraissent assez accessible. Travailler chacun de son côté sur des exercices différents nous permettait ainsi de gagner du temps. Lorsque nous travaillions depuis chez nous, nous restions en contact via la plateforme discord pour s'aider mutuellement et pour se partager nos avancées.

La seule aide exterieure dont nous avons eu besoin est de la documentation trouvable sur internet pour comprendre comment utiliser la classe "map" (pour les dictionnaires de l'exercice 12).La seule aide exterieure dont nous avons eu besoin est de la documentation trouvable sur internet pour comprendre comment utiliser la classe "map" (pour les dictionnaires de l'exercice 12).

+++

## Prise de recul

Lubin :Les difficultés rencontrées étaient principalement de bien comprendre comment s'y prendre pour traiter les données dans les différents fichiers, txt et csv. 

Ce projet nous a surtout appris à travailler sur des exercices longs, de moins en moins guidés. Cela nous a également servi à nous rendre compte de la grande quantité de données en libre accès sur le net, qui peuvent être utilisées pour de multiples raisons.

Si nous devions refaire le projet, nous ne nous y prendrions peut-être pas de cette manière là pour répartir le travail. Effectivement, sur ce projet assez simple et assez guidé, la répartition du travail que l'on a faite était adaptée, mais sur des projets plus difficiles, nous aurions eu du mal à bien comprendre les différents exercices si nous ne les avions pas fait simultanément, ensemble.

Rémi : Si je devais refaire quelque chose dans ce projet, je pense que j'essayerais de rendre mes programmes plus clairs. Je pourrais égalemment essayer de les optimiser, mais cela rend (selon moi) le code moins lisible et je m'y perds plus facilement. Enfin, j'aurais pu envisager d'essayer la gestion des erreurs. 

Je dois dire avoir beaucoup apprécié le fait d'implanter des fonctions sans savoir quand elles me serviraient (paradoxalement ?) mais de pouvoir y recourir par la suite, car je n'aurais pas pensé par moi-même à faire toutes ces fonctions pour alléger les fonctions main. Ainsi, les derniers exercices nécessitent en réalité très peu de code, mais plutôt plus de logique, pour savoir quelles fonctions utiliser.
