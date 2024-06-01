---
jupytext:
  notebook_metadata_filter: rise
  text_representation:
    extension: .md
    format_name: myst
    format_version: 0.13
    jupytext_version: 1.14.5
kernelspec:
  display_name: Python 3 (ipykernel)
  language: python
  name: python3
rise:
  auto_select: first
  autolaunch: false
  backimage: fond.png
  centered: false
  controls: false
  enable_chalkboard: true
  height: 100%
  margin: 0
  maxScale: 1
  minScale: 1
  scroll: true
  slideNumber: true
  start_slideshow_at: selected
  transition: none
  width: 90%
---

+++ {"slideshow": {"slide_type": "slide"}}

# Entrée et sortie de ville

- Binôme: Lubin Longuépée, Aidan Barouk
- Adresses mails: lubin.longuepee@universite-paris-saclay.fr / aidan.barouk@universite-paris-saclay.fr
- [Dépôt GitLab](https://gitlab.dsi.universite-paris-saclay.fr/lubin.longuepee/L1InfoInitiationScienceDonnees-2022-2023-Semaine8)

+++ {"slideshow": {"slide_type": "slide"}}

## Jeu de données

+++

Pour ce projet, nous avons voulu choisir un jeu de données sortant de l'ordinaire par rapport à ce qui a pu ếtre fait lors du premier projet. L'idée de notre projet nous est venue en résolvant des CAPTCHA sur internet. Nous nous sommes rendus compte qu'une grande partie de ces CAPTCHA nous demandent de reconnaitre des différetns éléments sur la route, comme des panneaux, des feux rouges, des voitures sur la route, des piétons... Nous nous sommes rapellé que certains de ces CAPTCHA servaient en fait de main d'oeuvre gratuite pour entrainer des intelligences artificielles à reconnaitre les différents éléments présents sur les routes. Ces intelligences artificielles sont en fait celles qui ont pour objectif de conduire des véhicules de manière autonome. Nous avons donc voulu reprendre un des éléments présents sur ces captchas pour voir s'il est difficile de différencier deux éléments.

+++

### Import

```{code-cell} ipython3
# Automatically reload code when changes are made
%load_ext autoreload
%autoreload 2

import os
from PIL import Image, ImageDraw, ImageFont
import matplotlib.pyplot as plt
%matplotlib inline
import scipy

from scipy import signal
import pandas as pd
import seaborn as sns
from glob import glob as ls
import sys
from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import balanced_accuracy_score as sklearn_metric

from utilities import *
from intro_science_donnees import data
from intro_science_donnees import *
```

### Visualisation

```{code-cell} ipython3
sklearn_model = KNeighborsClassifier(n_neighbors=3)
performances = pd.DataFrame(columns = ['Traitement', 'perf_tr', 'std_tr', 'perf_te', 'std_te'])
performances
```

####

```{code-cell} ipython3
dataset_dir = 'data'

images = load_images(dataset_dir, "*.jpg")
sample = list(images[:15]) + list(images[-15:])
image_grid(sample)
```

+++ {"slideshow": {"slide_type": "slide"}}

## Prétraitement

+++

Tout d'abord,pour une question de gain de temps, nous avons préféré crop toutes les images manuellement plutôt que de créer une fonction qui arrive à toutes les recadrer automatiquement. En effet, nos images n'ont pas du tout la même forme, la même taille, la même orientation, les mêmes détails importants ou encore les mêmes angles de vus. Créer une fonction qui aurait crop toutes les images automatiquement aurait demandé énormément de temps, ce qui n'est pas utile pour un jeu de donnée de seulement 30 images.

```{code-cell} ipython3
images[15] = images[15].crop(box=(5, 60, 250, 115))
images[16] = images[16].crop(box=(10, 30, 250, 160))
images[17] = images[17].crop(box=(20, 55, 220, 155))
images[18] = images[18].crop(box=(80, 65, 160, 110))
images[19] = images[19].crop(box=(50, 50, 220, 130))
images[20] = images[20].crop(box=(40, 100, 140, 145))
images[21] = images[21].crop(box=(20, 50, 160, 130))
images[22] = images[22].crop(box=(10, 60, 240, 140))
images[23] = images[23].crop(box=(10, 35, 260, 100))
images[24]= images[24].crop(box=(10, 30, 170, 80))
images[25] = images[25].crop(box=(50, 60, 210, 120))
images[26] = images[26].crop(box=(20, 40, 230, 90))
images[27] = images[27].crop(box=(20, 30, 240, 125))
images[28] = images[28].crop(box=(50, 40, 240, 120))
images[29] = images[29].crop(box=(10, 50, 240, 100))
images[0]=images[0].crop(box=(60,90,170,140))
images[1]=images[1].crop(box=(20,50,230,150))
images[2]=images[2].crop(box=(70,30,240,80))
images[3]=images[3].crop(box=(140,40,230,80))
images[4] = images[4].crop(box=(5, 40, 250, 130))
images[5] = images[5].crop(box=(4, 25, 260, 105))
images[6] = images[6].crop(box=(5, 40, 240, 120))
images[7] = images[7].crop(box=(40, 50, 220, 130))
images[8] = images[8].crop(box=(10, 30, 260, 90))
images[9] = images[9].crop(box=(20, 45, 210, 160))
images[10] = images[10].crop(box=(130, 45, 220, 80))
images[11] = images[11].crop(box=(5, 40, 240, 110))
images[12] = images[12].crop(box=(40, 20, 255, 70))
images[13] = images[13].crop(box=(20, 45, 230, 150))
images[14] = images[14].crop(box=(30, 80, 150, 120))
os.makedirs('clean_data', exist_ok=True)
for name, img in images.items():
    img.save(os.path.join('clean_data', os.path.splitext(name)[0]+".png"))
dataset_dir = 'clean_data'

images = load_images(dataset_dir, "*.png")
sample = list(images[:15]) + list(images[-15:])
image_grid(sample)
```

Essayons déjà d'appliquer le classificateur KNN à notre jeu de donnée:

```{code-cell} ipython3
df_raw = images.apply(image_to_series)
df_raw['class'] = df_raw.index.map(lambda name: 1 if name[0] == 'a' else -1)
# Validation croisée
p_tr, s_tr, p_te, s_te = df_cross_validate(df_raw.fillna(0), sklearn_model, sklearn_metric)
metric_name = sklearn_metric.__name__.upper()
print("AVERAGE TRAINING {0:s} +- STD: {1:.2f} +- {2:.2f}".format(metric_name, p_tr, s_tr))
print("AVERAGE TEST {0:s} +- STD: {1:.2f} +- {2:.2f}".format(metric_name, p_te, s_te))
performances.loc[0] = ["Images brutes", p_tr, s_tr, p_te, s_te]
performances.style.format(precision=2).background_gradient(cmap='Blues')
```

Essayons my_foreground_filter. On remarque que cela fonctionne plutôt bien. Nous verrons dans la suite si cela est utile. On remarque toutefois que certains panneaux ont l'interieur en blanc et les écritures et barres en noir alors que pour les autres images, c'est l'inverse.

```{code-cell} ipython3
def my_foreground_filter(img):
    foreground = foreground_redness_filter(img, theta=.6)
    foreground = scipy.ndimage.gaussian_filter(foreground, sigma=.05)
    return foreground
image_grid([my_foreground_filter(img)
            for img in sample])
```

Réutilisons maintenant tous les éléments précédents et ajoutons le recadrage au millieu de l'image. On remarque que le recadrage ne marche pas du tout. En effet, nos images sont déjà manuellement croppées et elles n'ont pas du tout les mêmes angles de vus, les mêmes formes, orientations ect. Le recadrage n'est donc pas adapté pour notre jeu de donnée.

```{code-cell} ipython3
def my_preprocessing(img):
    """
    Prétraitement d'une image
    
    - Calcul de l'avant plan
    - Mise en transparence du fond
    - Calcul du centre
    - Recadrage autour du centre
    """
    foreground = my_foreground_filter(img)
    img = transparent_background(img, foreground)
    coordinates = np.argwhere(foreground)
    if len(coordinates) == 0: # Cas particulier: il n'y a aucun pixel dans l'avant plan
        width, height = img.size
        center = (width/2, height/2)
    else:
        center = (np.mean(coordinates[:, 1]), np.mean(coordinates[:, 0]))
    img = crop_around_center(img, center)
    return img
```

```{code-cell} ipython3
clean_images = [my_preprocessing(images[i]) for i in range(30)]
clean_sample = list(clean_images[:15]) + list(clean_images[-15:])
image_grid(clean_sample)
```

```{code-cell} ipython3
#Sauvegarde intermédiaire
# conversion
df_clean = images.apply(image_to_series)
df_clean=df_clean.fillna(0)
# ajout des étiquettes
df_clean['class'] = df_clean.index.map(lambda name: 1 if name[0] == 'a' else -1)
df_clean.to_csv('clean_data.csv')
df_clean = pd.read_csv('clean_data.csv', index_col=0)
df_clean
```

+++ {"slideshow": {"slide_type": "slide"}}

## Extraction d'attributs

+++

On remarque que sur nos panneaux, l'élément principal permettant de distinguer les deux catégories est la présence d'une barre rouge en diagonale dans le panneau. Et il se trouve que l'attribut redness, appliqué à notre jeu de données, met très bien en évidence cette diagonale, ce qui permet d'identifier facilement les deux catégories. L'avantage de cet attribut est qu'il fait disparraitre les écritures dans les panneaux, qui peuvent empecher de bien distinguer les deux types.

```{code-cell} ipython3
image_grid([redness_filter(img)
            for img in sample])
```

```{code-cell} ipython3

```

On peut maintenant essayer de réutiliser les attributs crées lors des semaines précédentes:

```{code-cell} ipython3
#La cellule ci-dessous sert juste à ne pas faire buguer df_features
dataset_dir = 'data'

images = load_images(dataset_dir, "*.jpg")
images[15] = images[15].crop(box=(5, 60, 250, 115))
images[16] = images[16].crop(box=(10, 30, 250, 160))
images[17] = images[17].crop(box=(20, 55, 220, 155))
images[18] = images[18].crop(box=(80, 65, 160, 110))
images[19] = images[19].crop(box=(50, 50, 220, 130))
images[20] = images[20].crop(box=(40, 100, 140, 145))
images[21] = images[21].crop(box=(20, 50, 160, 130))
images[22] = images[22].crop(box=(10, 60, 240, 140))
images[23] = images[23].crop(box=(10, 35, 260, 100))
images[24]= images[24].crop(box=(10, 30, 170, 80))
images[25] = images[25].crop(box=(50, 60, 210, 120))
images[26] = images[26].crop(box=(20, 40, 230, 90))
images[27] = images[27].crop(box=(20, 30, 240, 125))
images[28] = images[28].crop(box=(50, 40, 240, 120))
images[29] = images[29].crop(box=(10, 50, 240, 100))
images[0]=images[0].crop(box=(60,90,170,140))
images[1]=images[1].crop(box=(20,50,230,150))
images[2]=images[2].crop(box=(70,30,240,80))
images[3]=images[3].crop(box=(140,40,230,80))
images[4] = images[4].crop(box=(5, 40, 250, 130))
images[5] = images[5].crop(box=(4, 25, 260, 105))
images[6] = images[6].crop(box=(5, 40, 240, 120))
images[7] = images[7].crop(box=(40, 50, 220, 130))
images[8] = images[8].crop(box=(10, 30, 260, 90))
images[9] = images[9].crop(box=(20, 45, 210, 160))
images[10] = images[10].crop(box=(130, 45, 220, 80))
images[11] = images[11].crop(box=(5, 40, 240, 110))
images[12] = images[12].crop(box=(40, 20, 255, 70))
images[13] = images[13].crop(box=(20, 45, 230, 150))
images[14] = images[14].crop(box=(30, 80, 150, 120))
os.makedirs('clean_data', exist_ok=True)
for name, img in images.items():
    img.save(os.path.join('clean_data', os.path.splitext(name)[0]+".png"))
dataset_dir = 'clean_data'

images = load_images(dataset_dir, "*.png")
def my_foreground_filter(img):
    foreground = foreground_redness_filter(img, theta=.6)
    foreground = invert_if_light_background(foreground)
    foreground = scipy.ndimage.gaussian_filter(foreground, sigma=.05)
    return foreground
```

```{code-cell} ipython3
def my_preprocessing(img):
    """
    Prétraitement d'une image
    
    - Calcul de l'avant plan
    - Mise en transparence du fond
    - Calcul du centre
    - Recadrage autour du centre
    """
    foreground = my_foreground_filter(img)
    img = transparent_background(img, foreground)
    coordinates = np.argwhere(foreground)
    if len(coordinates) == 0: # Cas particulier: il n'y a aucun pixel dans l'avant plan
        width, height = img.size
        center = (width/2, height/2)
    else:
        center = (np.mean(coordinates[:, 1]), np.mean(coordinates[:, 0]))
    #img = crop_around_center(img, center)
    return img
```

```{code-cell} ipython3
clean_images = images.apply(my_preprocessing)
clean_sample = list(clean_images[:15]) + list(clean_images[-15:])
```

```{code-cell} ipython3
df_features = pd.DataFrame({'redness': clean_images.apply(redness),
                            'greenness': clean_images.apply(greenness),
                            'blueness': clean_images.apply(blueness),
                            'elongation': clean_images.apply(elongation),
                            'perimeter': clean_images.apply(perimeter),
                            'surface': clean_images.apply(surface),
                            'blackness' : clean_images.apply(blackness),
                            'espace_milieu' : clean_images.apply(espacemilieu)
                           })
df_features
```

On remarque que l'écart type est très grand, on peut donc normaliser notre table:

```{code-cell} ipython3
epsilon = sys.float_info.epsilon
df_features = (df_features - df_features.mean())/(df_features.std() + epsilon) # normalisation 
df_features.describe() # nouvelles statistiques de notre jeu de donnée
```

```{code-cell} ipython3
df_features["class"] = df_clean["class"] #On rajoute l'attribut class pour pouvoir classer nos images en deux catégories
df_features[df_features.isna()] = 0 #Pour éviter d'avoir des cases vides (Na)
df_features.style.background_gradient(cmap='coolwarm') #Carte de chaleur
```

On applique nos attributs au classificateur :

```{code-cell} ipython3
# Validation croisée (LENT)
p_tr, s_tr, p_te, s_te = df_cross_validate(df_features, sklearn_model, sklearn_metric,)
metric_name = sklearn_metric.__name__.upper()
print("AVERAGE TRAINING {0:s} +- STD: {1:.2f} +- {2:.2f}".format(metric_name, p_tr, s_tr))
print("AVERAGE TEST {0:s} +- STD: {1:.2f} +- {2:.2f}".format(metric_name, p_te, s_te))
```

```{code-cell} ipython3
performances.loc[2] = ["6 attributs ad-hoc", p_tr, s_tr, p_te, s_te]
performances.style.format(precision=2).background_gradient(cmap='Blues')
```

Essayons de selectionner les attributs les plus corrélés. Pour cela, affichons la carte de chaleure normalisée

```{code-cell} ipython3
# Compute correlation matrix
corr = df_features.corr()
corr.style.format(precision=2).background_gradient(cmap='coolwarm')
```

```{code-cell} ipython3
s = df_features.iloc[3].replace({'redness': 4})
header = ['R','G','B','M=maxRGB', 'm=minRGB', 'C=M-m', 'R-(G+B)/2', 'G-B', 'G-(R+B)/2', 'B-R', 'B-(G+R)/2', 'R-G', '(G-B)/C', '(B-R)/C', '(R-G)/C', '(R+G+B)/3', 'C/V']

df_features_large = df_features.drop("class", axis = 1)

df_features_large = pd.concat([df_features_large, clean_images.apply(get_colors)], axis=1)

epsilon = sys.float_info.epsilon # epsilon
df_features_large = (df_features_large - df_features_large.mean())/(df_features_large.std() + epsilon) # normalisation 
df_features_large[df_features_large.isna()] = 0
df_features_large.describe() # nouvelles statistiques de notre jeu de donnée
    
    
df_features_large["class"] = df_clean["class"]
df_features_large
```

Affichons maintenant les performances de notre classificateur :

```{code-cell} ipython3
# Validation croisée (LENT)
p_tr, s_tr, p_te, s_te = df_cross_validate(df_features_large, sklearn_model, sklearn_metric)
metric_name = sklearn_metric.__name__.upper()
print("AVERAGE TRAINING {0:s} +- STD: {1:.2f} +- {2:.2f}".format(metric_name, p_tr, s_tr))
print("AVERAGE TEST {0:s} +- STD: {1:.2f} +- {2:.2f}".format(metric_name, p_te, s_te))
```

```{code-cell} ipython3
performances.loc[3] = ["23 attributs ad-hoc", p_tr, s_tr, p_te, s_te]
performances.style.format(precision=2).background_gradient(cmap='Blues')
```

La cellule ci-dessous fait en sorte de trier les attributs les plus corrélés en valeur absolue. On affiche donc les attributs dans l'ordre décroissant (ceux qui corrèlent le plus en premier)

```{code-cell} ipython3
# Sort by the absolute value of the correlation coefficient
corr_large = df_features_large.corr()
corr_large.style.format(precision=2).background_gradient(cmap='coolwarm')
sval = corr_large['class'][:-1].abs().sort_values(ascending=False)
ranked_columns = sval.index.values
print(ranked_columns) 
```

La cellule ci-dessous selectionne les 5 attributs les plus corrélés en valeur absolue et les affiche dans un pair-plot

```{code-cell} ipython3
col_selected = ranked_columns[0:5]
df_features_final = pd.DataFrame.copy(df_features_large)
df_features_final = df_features_final[col_selected]

df_features_final['class'] = df_features_large["class"]
g = sns.pairplot(df_features_final, hue="class", markers=["o", "s"], diag_kind="hist")
```

Essayons maintenant de voir quel nombre d'attributs est le plus optimal dans notre cas:

```{code-cell} ipython3
# On importe notre modèle
from sklearn.metrics import balanced_accuracy_score as sklearn_metric
sklearn_model = KNeighborsClassifier(n_neighbors=3)
feat_lc_df, ranked_columns = feature_learning_curve(df_features_large, sklearn_model, sklearn_metric)
#feat_lc_df[['perf_tr', 'perf_te']].plot()
plt.errorbar(feat_lc_df.index+1, feat_lc_df['perf_tr'], yerr=feat_lc_df['std_tr'], label='Training set')
plt.errorbar(feat_lc_df.index+1, feat_lc_df['perf_te'], yerr=feat_lc_df['std_te'], label='Test set')
plt.xticks(np.arange(1, 22, 1)) 
plt.xlabel('Number of features')
plt.ylabel(sklearn_metric.__name__)
plt.legend(loc='lower right');
```

On remarque que 14 est le nombre d'attributs pour lequel le score est le plus élevé.

+++

Sauvegardons maintenant dans un autre fichier CSV nos données finales

```{code-cell} ipython3
df_features_final.to_csv('features_data.csv') # export des données dans un fichier
#df_features_final = pd.read_csv('features_data.csv')  # chargement du fichier dans le notebook
```

```{code-cell} ipython3
# Validation croisée
p_tr, s_tr, p_te, s_te = df_cross_validate(df_features_final, sklearn_model, sklearn_metric)
metric_name = sklearn_metric.__name__.upper()
print("AVERAGE TRAINING {0:s} +- STD: {1:.2f} +- {2:.2f}".format(metric_name, p_tr, s_tr))
print("AVERAGE TEST {0:s} +- STD: {1:.2f} +- {2:.2f}".format(metric_name, p_te, s_te))
```

```{code-cell} ipython3
performances.loc[4] = ["5 attributs par analyse de variance univarié", p_tr, s_tr, p_te, s_te]
performances.style.format(precision=2).background_gradient(cmap='Blues')
```

L'approche que l'on a fait jusqu'à présent est une approche univariée (on considère chaque attribut séparrement). Mais il peut être utile d'en regrouper par pair. En effet, certains attributs peuvent ne pas être corrélés individuellement mais l'être quand ils sont regroupés par pair. Essayons l'approche multi-variée

```{code-cell} ipython3
best_perf = -1
std_perf = -1
best_i = 0
best_j = 0
for i in np.arange(5): 
    for j in np.arange(i+1,5): 
        df = df_features_large[[ranked_columns[i], ranked_columns[j], 'class']]
        p_tr, s_tr, p_te, s_te = df_cross_validate(df_features_large, sklearn_model, sklearn_metric)
        if p_te > best_perf: 
            best_perf = p_te
            std_perf = s_te
            best_i = i
            best_j = j
            
metric_name = sklearn_metric.__name__.upper()
print('BEST PAIR: {}, {}'.format(ranked_columns [best_i], ranked_columns[best_j]))
print("AVERAGE TEST {0:s} +- STD: {1:.2f} +- {2:.2f}".format(metric_name, p_te, s_te))
```

On remarque que dans notre  cas, cette approche n'est pas très utile puisque le score n'est pas mieux. On remarque également que la paire d'attribut ayant les meilleures performances est constituée en fait des deux attributs les plus performants indidivuellement, ce qui n'aide pas à améliorer les performances.

+++ {"slideshow": {"slide_type": "slide"}}

## Comparaison des classificateurs

```{code-cell} ipython3
df_features = pd.read_csv('features_data.csv', index_col=0)
df_features.describe()
```

Nous allons utiliser les différents classificateurs de la librairie sickit learn:

```{code-cell} ipython3
from sklearn.neural_network import MLPClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn.svm import SVC
from sklearn.gaussian_process import GaussianProcessClassifier
from sklearn.gaussian_process.kernels import RBF
from sklearn.tree import DecisionTreeClassifier
from sklearn.ensemble import RandomForestClassifier, AdaBoostClassifier
from sklearn.naive_bayes import GaussianNB
from sklearn.discriminant_analysis import QuadraticDiscriminantAnalysis

model_name = ["Nearest Neighbors", "Linear SVM", "RBF SVM", "Gaussian Process",
         "Decision Tree", "Random Forest", "Neural Net", "AdaBoost",
         "Naive Bayes", "QDA"]
model_list = [
    KNeighborsClassifier(3),
    SVC(kernel="linear", C=0.025, probability=True),
    SVC(gamma=2, C=1, probability=True),
    GaussianProcessClassifier(1.0 * RBF(1.0)),
    DecisionTreeClassifier(max_depth=10),
    RandomForestClassifier(max_depth=10, n_estimators=10, max_features=1),
    MLPClassifier(alpha=1, max_iter=1000),
    AdaBoostClassifier(),
    GaussianNB(),
    QuadraticDiscriminantAnalysis()]
```

```{code-cell} ipython3
from sklearn.metrics import balanced_accuracy_score as sklearn_metric
compar_results = systematic_model_experiment(df_features, model_name, model_list, sklearn_metric)
compar_results.style.set_precision(2).background_gradient(cmap='Blues')
```

```{code-cell} ipython3
compar_results[['perf_tr', 'perf_te']].plot.bar()
plt.ylim(0.5, 1)
plt.ylabel(sklearn_metric.__name__);
```

On remarque que random forest est le classificateur le plus efficace

```{code-cell} ipython3
analyze_model_experiments(compar_results)
```

On remarque que le seul attribut sous-entrainé est QDA tandis que les attributs sur-entrainés sont RBF SVM, Gaussian Process, Decision Tree et Random foret, tandis que les autres ne sont ni sur-entrainés ni sous-entrainés.

+++

Comité de classificateur et d'incertitude :

```{code-cell} ipython3
class ClassifierCommittee():
    def __init__(self, model_list):
        self.model_list = model_list
        
    def fit(self,X,y):
        for model in self.model_list:
            model.fit(X,y)
    def predict(self,X):
        predictions = []
        for model in self.model_list:
            predictions.append(model.predict(X))
        predictions = np.mean(np.array(predictions),axis = 0)
        results = []
        for v in predictions:
            if v < 0:
                results.append(-1)
            else:
                results.append(1)
        return np.array(results)
    
    def predict_proba(self,X):
        predictions = []
        for model in self.model_list:
            predictions.append(model.predict_proba(X))
        return np.swapaxes(np.array(predictions), 0, 1)
    def score(self,X):
        scores = []
        for model in self.model_list:
            scores.append(model.score(X,y))
        return np.swapaxes(np.array(scores), 0, 1)
```

```{code-cell} ipython3
commitee = ClassifierCommittee(model_list)
perf_tr, std_tr, perf_te, std_te = df_cross_validate(df_features, commitee, sklearn_metric)
print(perf_te, std_te)
```

On remarque que les performances obtenues sont plutôt correctes.

+++

Nous allons maintenant essayer de déterminer lesquelles des images sont les plus incertaines grâce à l'entropie de Shannon

```{code-cell} ipython3
X = df_features.iloc[:, :-1].to_numpy()
Y = df_features.iloc[:, -1].to_numpy()
commitee.fit(X, Y)
prediction_probabilities = commitee.predict_proba(X)
from scipy.stats import entropy
entropies_per_classifier = entropy(np.swapaxes(prediction_probabilities, 0, 2))
entropies = np.mean(entropies_per_classifier, axis = 0)
df_uncertainty = pd.DataFrame({"images" : images,
                           "entropy" : entropies})
df_uncertainty = df_uncertainty.sort_values(by=['entropy'],ascending=False)
```

Voici les 10 images avec l'entropie la plus élevée, donc l'incertitude la plus élevée

```{code-cell} ipython3
uncertain_aleatoric_images = df_uncertainty['images'].tolist()
image_grid(uncertain_aleatoric_images[:10])
```

Et les 10 images avec l'incertitude la moins élevée

```{code-cell} ipython3
image_grid(uncertain_aleatoric_images[-10:])
```

+++ {"slideshow": {"slide_type": "slide"}}

## Résultats

### Observations

+++

On a éssayé plusieurs attributs, plusieurs classificateurs. Avec le classificateur KNN, on peut observer ces résultats, en fonction du nombre d'attributs :

```{code-cell} ipython3
performances.style.format(precision=2).background_gradient(cmap='Blues')
```

Concernant les classificateurs, on peut observer ces performances pour chacun de ceux-ci:

```{code-cell} ipython3
compar_results.style.set_precision(2).background_gradient(cmap='Blues')
```

### Interprétations

+++

En ce qui concerne les attributs, on peut étonnamment remarquer que le nombre d'attributs n'est pas synonyme de performance optimale. En effet, choisir des attributs bien corrélés au jeu de donnée permet d'être plus performant que de prendre tous les attributs que l'on connait.
En ce qui concerne les classificateurs, on remarque qu'ils ont tous des performances individuelles correctes même si certains sont plus performants que d'autre. Certains d'entre eux ne sont pas adapté parfaitement à notre jeu de données car certains sont surentrainés tandis que d'autres sont sous-entrainés. Enfin, L'analyse multi-variée n'a dans notre cas que peu d'interêt car les classificateurs ne forment pas des paires utiles pour discriminer les deux étiquettes.

+++ {"slideshow": {"slide_type": "slide"}, "jp-MarkdownHeadingCollapsed": true}

## Discussion 
Il existe des biais dans notre jeu de données. En effet, toute nos images sont des photos prises de jour. Il n'y en a aucune prise de nuit, qui pourrait donc changer la luminosité. De plus les images sont plus ou moins prises de face, même si certaines photos ont des angles un peu différent, on voit toujours correctement l'avant du panneau. On peut aussi remarquer que les photos que l'on a prise sont des photos du type commun des panneaux mais il existe certains panneaux, dans des villages nottamment, qui sont jaunes ou n'ont pas une forme parfaitement rectangulaire. Enfin, le biais majeur est que nos images sont croppées parfaitement au centre, de façon à ce qu'on ne voit que le panneau mais en réalité, les panneaux sont dans un cadre qui peut énormément varier (forêt, champ, ville, route...) : les classificateurs peuvent donc être trompés par ces fonds.

Nous avons dans ce projet rencontré de diverses difficultés.
La première a été d'utiliser le programme fourni pour recentrer toutes les images automatiquement. En effet, notre jeu d'images n'était pas adapté à ces fonctions donc on a du crop toutes nos images manuellement.
De plus, nous n'avions pas initialement compris que nous devions reconvertir nos images en .png pour utiliser les différentes fonction de utilities.py (puisque nous avions besoin du 4ème canal des pixels, correspondant à la transparence)
Enfin, la dernière difficulté majeure a été lors de la création du rapport: des incohérences sont apparues et ont fait planter le code que nous avons réutilisé dans le rapport pour montrer notre travail. Certaines cellules ne voulaient pas s'executer alors que dans les feuilles précédentes, elles fonctionnaient parfaitement et les différents paramètres utilisés n'ont pas changé. La seule solution que nous avons trouvé a été de recopier du code inutilement pour passer outre ces problèmes.

+++

## Conclusion

+++

En conclusion, nous sommes de désormais capable de différencier plutôt correctement les panneaux d'entrée et de sortie de ville, malgré quelques difficultés. On peut donc maintenant s'amuser à faire résoudre des captchas (très spécifiques...) à notre programme pour entrainer de potentielles IA.
