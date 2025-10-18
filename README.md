Voici mon git contenant tous les fichiers que j'ai ou que je vais utiliser durant la création d'un petit jeu personelle.
Je code le jeu en C, en utilisant pour l'inteface la biblioteque graphique SDL (plus précisemment SDL2)
De plus, vous trouverez ci-dessous un descriptif du jeu et de ses méchaniques


--------------------------------------------------------------------------------------------------------------------------------------------------------

Le jeu possède une interface simple, comme un site web avec une en-tête globale, et plusieurs pages entre lesquelles on peut naviguer.
Voici comment il se décompose.

1. L'en-tête

    Il est générale à tout le jeu, peu importe où se trouve le joueur, il sera toujours affiché. On peut y trouver :
      - Le nom d jeu : Blacksmith Master
      - Le solde du joueur
      - Le temps (jour de la semaine + heure) : le temps avance de +1H à un pas fixe de temps réel, et le salaire des employé est versé tout les jours à minuit
      - Le nom des différentes pages sur lesquelles on peut cliquer pour s'y diriger (le nom de la page actuelle est légerement grisé)

2. La Forge
   
      C'est la page où le joueur peut organiser la production d'objets. On peut y trouver :
        - La gestion de la production en elle même : menu déroulant où l'on peut voir les objets qu'on a débloqué, avec --> prix de vente, matériaux requis, tps de fabriation pour 1 forgeron, nb de forgeron assigné.
        - La partie "Embauche", où l'on peut voir le nb de forgeron que l'on emploie, la somme des salaires, et la possibilité d'embaucher (avec un coût initial qui augmente à chaque embauche) ou licencier un forgeron.

3. Le Magasin
   
      C'est la page pù l'on peut gérer notre facade de magasin. On peut embaucher des venteur et acheter de nouvelles étagère pour booster les ventes, voir la probabilité d'achat et la quantité de chaque objet etc. On peut y voir
        - Le catalogue des objets en vente, avec prix, quantité et probabilité de vente (inversemment proportionnel au prix de l'objet). A noter q'un client regarder chaque objet individuellement. Il est donc dans l'interet du
            joueur que d'avoir une production diversifié pour maximiser les profits
       - La partie "Améliorations", où l'on peut :
           - Embaucher un vendeur : augmentera la probabilité de vente de tout les objets (proportionnelement à la probabilité de base). ATTENTION nombre limité de vendeur. (Idem que pour forgeron pour Embauche/Licenciement)
           - Acheter de nouvelles étagères, pour attirer plus de clients par jour
         
4. Les Mines

       A débloqué dans l'arbre de technolgie. Permet, en l'échange de l'embauche et la paye de mineurs, d'avoir des matières premières gratuitement (fer, argent et or).
         - La partie "Mines", ou l'on peut voir la mine de fer, d'or et d'argent (si pas encore débloqué, est grisé) avec : nb de mineurs assigné, le tps d'extraction pour 1 mineur d'une unité de fer/argent/or, et la production
             totale par jour
        - La partie "Embauche" de mineurs, idem que pour les forgerons

5. Les Stocks

       Menu déroulant ou l'on peut voir un graphique au cours du temps de, dans l'ordre :
         - L'or du joueur et des matières premières (fer, argent, or) + boutons pour acheter des matières première avec le prix à l'unité affiché pour chacun
         - Le stocks de tout les objets
           
9. L'Arbre de technologie
   
      Un arbre technologique séparé en deux partie : les mines, et les objets, où l'on peut, en échange d'or, débloquer de nouvelle technologies
        - Les mines : d'abord la mine de fer, puis d'argent, puis d'or
        - Les objets : idem que pour les mines. Exemple : dague(débloqué de base) --> masse --> bouclier --> épée courte --> épée longue --> hache de guerre --> bague en argent --> ... --> collier en or --> ...
