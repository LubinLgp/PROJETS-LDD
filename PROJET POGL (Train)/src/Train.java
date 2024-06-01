import java.util.*;

// Classe représentant le train avec ses wagons et le Marshall
public class Train {
    public static int NB_WAGONS; // Nombre de wagons
    private static double NERVOSITE_MONTRE; //proba que la personne possédant la montre se deplace à chaque action
    private final Wagon[] wagons;
    private final Marshall marshall;
    private Bandit[] bandits;
    private double sommeTotale; //somme totale pour gagner le jeu

    public Train(int nbJoueurs, String[] names, String difficulty) {
        sommeTotale = 0;
        double nerv; //correspond à NERVOSITE_MARSHALL (augmente en fonction de la difficulté)
        int nbBalle; //correspond au nombre de balles de chaque bandit (diminue en fonction de la difficulté)
        int nbAction; //correspond au nombre d'action de chaque bandit à chaque tour (augmente en fonction de la difficulté)
        int nbTour; //correspond au nombre de tours à passer avec la montre pour gagner (augmente en fonction de la difficulté)
        double coefDifficulte; //correspond au coefficient rendant la somme à recolter pour gagner plus ou moins élevée selon la difficulté (+ elevée pour une difficultée plus dure)
        if (Objects.equals(difficulty, "Facile")) {
            NB_WAGONS = 6;
            nerv = 0.2;
            nbBalle = 6;
            nbAction = 3;
            NERVOSITE_MONTRE = 0.2;
            nbTour = 3;
            coefDifficulte=0.5;
        } else if (Objects.equals(difficulty, "Moyen")) {
            NB_WAGONS = 5;
            nerv = 0.35;
            nbBalle = 5;
            nbAction = 4;
            NERVOSITE_MONTRE = 0.3;
            nbTour = 3;
            coefDifficulte=0.6;
        } else {
            NB_WAGONS = 4;
            nerv = 0.5;
            nbBalle = 4;
            nbAction = 5;
            NERVOSITE_MONTRE = 0.4;
            nbTour = 4;
            coefDifficulte=0.75;
        }
        this.wagons = new Wagon[NB_WAGONS];

        this.marshall = new Marshall(nerv, nbAction);
        this.bandits = new Bandit[nbJoueurs];
        for (int i = 0; i < bandits.length; i++) {
            bandits[i] = new Bandit(names[i], nbBalle, nbAction, nbTour);
        }
        //Ici on convertit en liste pour "shuffle" la liste, car difficile avec un tableau
        ArrayList<Bandit> banditsList = new ArrayList<>(Arrays.asList(bandits));
        Collections.shuffle(banditsList);
        bandits = banditsList.toArray(new Bandit[0]); //on reconvertit en tableau
        for (int i = 0; i < NB_WAGONS; i++) {
            wagons[i] = new Wagon(i + 1);
        }
        for (Wagon w : wagons) {
            for (Butin b : w.getButins()) {
                if (!(b instanceof MontreDeLuxe)) {
                    sommeTotale += b.getValeur();
                }
            }
        }
        sommeTotale=sommeTotale*coefDifficulte;
    }

    public int getNB_WAGONS() {
        return NB_WAGONS;
    }

    public double getSommeTotale() {
        return sommeTotale;
    }

    public double getNervositeMontre() {
        return NERVOSITE_MONTRE;
    }

    public Wagon[] getWagons() {
        return wagons;
    }

    public Bandit[] getBandits() {
        return bandits;
    }

    public void setBandits(Bandit[] b) {
        this.bandits = b;
    }

    public Marshall getMarshall() {
        return marshall;
    }

    public boolean[] verifierRencontreMarshall(int posMarshall) { //verifie si le bandit est dans le meme wagon que le marshall, auquel cas il monte et lache un de ses butins aléatoirement
        if (getMarshall() == null) {
            System.out.println("Erreur: Le Marshall est null.");
            return new boolean[getBandits().length];
        }
        if (getBandits() == null) {
            System.out.println("Erreur: La liste des bandits est null.");
            return new boolean[getBandits().length];
        }
        int i = 0;
        boolean[] res = new boolean[getBandits().length];
        for (Bandit bandit : this.getBandits()) {
            if (bandit.getPositionWagon() == posMarshall && !bandit.isSurToit()) {
                banditRencontre(bandit);
                res[i] = true;
            }
            i++;
        }
        return res;
    }

    private void banditRencontre(Bandit bandit) {
        Random rand = new Random();
        if (!bandit.getButins().isEmpty()) {
            int index = rand.nextInt(bandit.getButins().size());
            Butin butin = bandit.getButins().remove(index);
            if (butin instanceof MontreDeLuxe) {
                bandit.remettreAZeroTour();
            }
            this.getWagons()[bandit.getPositionWagon() - 1].getButins().add(butin);
            System.out.println(bandit.getNom() + " a été rattrapé par le Marshall et lâche un butin de " + butin.getValeur());
        }
        bandit.deplacer(Direction.HAUT);
        System.out.println(bandit.getNom() + " se déplace immédiatement sur le toit.");
    }


}