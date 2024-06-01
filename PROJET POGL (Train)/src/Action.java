import java.util.Random;
import java.util.ArrayList;

public abstract class Action {
    protected individu cible;

    public Action(individu cible) {
        this.cible = cible;
    }

    public abstract void executer();
}
class ActionMonter extends Action {
    public ActionMonter(Bandit bandit) { super(bandit); }

    @Override
    public void executer() {
        cible.deplacer(Direction.HAUT);
    }
}
class ActionDescendre extends Action {
    public ActionDescendre(Bandit bandit) { super(bandit); }

    @Override
    public void executer() {
        cible.deplacer(Direction.BAS);
    }
}
class ActionAvancer extends Action {
    public ActionAvancer(individu bandit) { super(bandit); }

    @Override
    public void executer() {
        cible.deplacer(Direction.AVANT);
    }
}
class ActionReculer extends Action {
    public ActionReculer(individu bandit) { super(bandit); }

    @Override
    public void executer() {
        cible.deplacer(Direction.ARRIERE);
    }
}

class ActionBraquer extends Action {
    private final Train train; // Référence au train pour accéder aux wagons et aux butins

    public ActionBraquer(Bandit cible, Train train) {
        super(cible); // Appel au constructeur de la classe mère Action
        this.train = train;
    }

    @Override
    public void executer() {
        new Braquer(cible, train).executer();
    }
}
class ActionStatique extends Action {
    public ActionStatique(Marshall cible) {
        super(cible); // Appel au constructeur de la classe mère Action
    }
    @Override
    public void executer() {
    }
}
class ActionTir extends Action {
    private final Direction direction;
    private final Train train;

    public ActionTir(Bandit tireur, Direction direction, Train train) {
        super(tireur);
        this.direction = direction;
        this.train = train;
    }

    @Override
    public void executer() {
        // Casting du tireur pour accéder aux méthodes spécifiques à Bandit
        Bandit tireur = (Bandit) this.cible;

        ArrayList<Bandit> ciblesPotentielles = new ArrayList<>();

        // Identifier les cibles potentielles selon la direction en respectant les consignes données
        for (Bandit b : train.getBandits()) {
            boolean isCiblePotentielle = false;

            switch (direction) {
                case AVANT:
                    if (b.getPositionWagon()+1 == tireur.getPositionWagon() && b.isSurToit() == tireur.isSurToit() && !b.equals(tireur)) {
                        isCiblePotentielle = true;
                    }
                    break;
                case ARRIERE:
                    if (b.getPositionWagon()-1 == tireur.getPositionWagon() && b.isSurToit() == tireur.isSurToit() && !b.equals(tireur)) {
                        isCiblePotentielle = true;
                    }
                    break;
                case HAUT:
                    if (b.getPositionWagon() == tireur.getPositionWagon() && b.isSurToit() && !b.equals(tireur)) {
                        isCiblePotentielle = true;
                    }
                    break;
                case BAS:
                    if (b.getPositionWagon() == tireur.getPositionWagon() && !b.isSurToit() && !b.equals(tireur)) {
                        isCiblePotentielle = true;
                    }
                    break;
            }

            if (isCiblePotentielle) {
                ciblesPotentielles.add(b);
            }
        }

        // Tirer au hasard sur une des cibles potentielles, si possible
        if (!ciblesPotentielles.isEmpty()) {
            Random rand = new Random();
            Bandit victime = ciblesPotentielles.get(rand.nextInt(ciblesPotentielles.size()));

            // Faire lâcher un butin au hasard de la victime
            if (!victime.getButins().isEmpty()) {
                Butin butinLache = victime.getButins().remove(rand.nextInt(victime.getButins().size()));
                if(butinLache instanceof MontreDeLuxe){
                    victime.remettreAZeroTour();
                }
                // Ajouter le butin lâché à la position actuelle du bandit touché
                Wagon wagon = train.getWagons()[victime.getPositionWagon() - 1];
                wagon.getButins().add(butinLache);


                System.out.println(victime.getNom() + " a été touché et lâche " + butinLache.getClass().getSimpleName());
            } else {
                System.out.println(victime.getNom() + " a été touché mais n'a aucun butin à lâcher.");
            }
        } else {
            System.out.println("Aucune cible n'a été touchée.");
        }
        tireur.decrementerBallesRestantes();
    }


}