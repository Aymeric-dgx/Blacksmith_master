# Blacksmith Master — Présentation

Voici mon git contenant tous les fichiers que j'ai ou que je vais utiliser durant la création d'un petit jeu personnel.  
Je code le jeu en **C**, en utilisant pour l’interface la bibliothèque graphique **SDL2**.  
De plus, vous trouverez ci-dessous un descriptif du jeu et de ses mécaniques.

---

## Résumé du jeu

Vous êtes un propriétaire d'une forge durant le Moyen-Age. Vous pouvez embaucher des forgerons, des vendeurs, des mineurs, débloquer de nouvelles technoligies (mines et objets) dans l'arbre, observer l'évoltuions des différents stockes...
Il y'a également un système de temps propre au jeu. Vous pouvez voir dans un coin de l'écran quel jour de la semaine nous sommes ainsi que l'heure, et tout les jours à minuit vous devrez payer les salaires de vos employés.

## Structure du jeu

Le jeu possède une interface simple, comme un site web avec une en-tête globale, et plusieurs pages entre lesquelles on peut naviguer.  
Voici comment il se décompose :

---

### 1. L’en-tête

Il est général à tout le jeu : peu importe où se trouve le joueur, il sera toujours affiché.  
On peut y trouver :

- Le nom du jeu : **Blacksmith Master**
- Le solde du joueur
- Le temps (jour de la semaine + heure) :  
  Le temps avance de +1h à un pas fixe de temps réel, et le salaire des employés est versé tous les jours à minuit.
- Le nom des différentes pages sur lesquelles on peut cliquer pour s’y diriger (le nom de la page actuelle est légèrement grisé)

---

### 2. La Forge

C’est la page où le joueur peut organiser la production d’objets.  
On peut y trouver :

- **Gestion de la production** :  
  Menu déroulant où l’on peut voir les objets débloqués, avec :  
  → prix de vente, matériaux requis, temps de fabrication pour 1 forgeron, nombre de forgerons assignés.
- **Embauche** :  
  Permet de voir le nombre de forgerons employés, la somme des salaires, et d’embaucher ou licencier un forgeron.  
  (Le coût initial d’embauche augmente à chaque nouvelle embauche.)

---

### 3. Le Magasin

C’est la page où l’on peut gérer notre façade de magasin.  
On peut y trouver :

- La partie "Catalogue", où l'on peut voir le prix de vente ainsi que la probabilité de vente de chaque objet (inversement proportionnelle au prix).
- La partie "Magasin", où l'on peut embaucher des vendeurs (idem que pour les forgerons) et acheter des étagères

**Détails :**

- Un client regarde chaque objet individuellement, il est donc dans l’intérêt du joueur d’avoir une production diversifiée pour maximiser les profits.
- L'embauche d'un vendeur augmente la probabilité de vente de tous les objets (proportionnellement à leur probabilité de base). **ATENTION** Nombre limité de vendeurs, contrairement au forgerons
- Acheter de nouvelles étagères attirerera plus de clients par jour.

---

### 4. Les Mines

À débloquer dans l’arbre de technologie.  
Permet, en échange de l’embauche et de la paie de mineurs, d’obtenir des matières premières gratuitement (**fer, argent, or**).

- **Mines** :  
  On peut voir les mines de fer, d’or et d’argent (si pas encore débloquées, elles sont grisées) avec :  
  → nombre de mineurs assignés, temps d’extraction pour 1 mineur, production totale par jour.
- **Embauche de mineurs** :  
  Fonctionne de la même manière que pour les forgerons.

---

### 5. Les Stocks

Menu déroulant où l’on peut voir un graphique des quantité au cours du temps de :

- L’or du joueur et des matières premières (fer, argent, or)  
  + boutons pour acheter des matières premières (prix unitaire affiché)
- Puis dans une deuxieme section en dessous, le stock de tous les objets

---

### 6. L’Arbre de technologie

Un arbre technologique séparé en deux parties : **les mines** et **les objets**, où l’on peut, en échange d’or, débloquer de nouvelles technologies.

- **Mines** : mine de fer → argent → or  
- **Objets** :  
  Exemple :  
  `dague (débloquée de base)` → `masse` → `bouclier` → `épée courte` → `épée longue` → `hache de guerre` → `bague en argent` → `...` → `collier en or` → `...`
