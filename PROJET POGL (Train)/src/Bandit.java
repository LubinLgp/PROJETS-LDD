import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Queue;

public class Bandit extends individu {
    private final ArrayList<Butin> butins; //liste des butins ramassés par le bandit
    private final Queue<Action> actionsPlanifiees; //la "pile" des actions du bandit dans l'ordre de planification
    private int actionsRestantes; // Ce champ garde le suivi du nombre d'actions restantes pour le bandit
    private int ballesRestantes; //nombre de balles restantes dans le chargeur
    protected int NB_BALLES; //nombre de balles au départ
    private int nbTourAvecMontre=0; //nombre de tours (cycle d'action) où le bandit possède la montre
    private final int nombreTourPourWin; //definit le nombre de tour pour gagner en gardant la montre
    public Bandit(String nom, int nbBalle,int nbAction, int nbTour) {
        super(nom, Train.NB_WAGONS, true,nbAction); // Bandit commence toujours au dernier wagon et sur le toit
        NB_BALLES=nbBalle;
        this.butins = new ArrayList<>();
        this.nombreTourPourWin=nbTour;
        this.actionsPlanifiees = new LinkedList<>();
        this.ballesRestantes = NB_BALLES;
        this.actionsRestantes = NB_ACTIONS;
    }
    public boolean possedeMontre(){
        for (Butin butin : butins) {
            if (butin instanceof MontreDeLuxe) { // Vérifier si le butin est une instance de MontreDeLuxe
                return true;
            }
        }
        return false;
    }
    public int getNbTourAvecMontre(){return nbTourAvecMontre;}
    public int getSommeTotale(){
        int res=0;
        for(Butin b:butins){
            if(! (b instanceof MontreDeLuxe)){ //la montre ne compte pas dans la somme totale
                res+=b.getValeur();
            }
        }
        return res;
    }
    public int getNombreTourPourWin(){return nombreTourPourWin;}
    public void decrementerBallesRestantes(){
        ballesRestantes--;
    }
    public void remettreAZeroTour(){nbTourAvecMontre=0;}
    public void incrementeTour(){nbTourAvecMontre++;}
    public void planifierAction(Action action) {
        if (actionsRestantes > 0) {
            actionsPlanifiees.add(action);
        }
    }
    public void decrementerActionsRestantes() {
        if (actionsRestantes > 0) {
            actionsRestantes--;
        }
    }

    public int getActionsRestantes() {
        return actionsRestantes;
    }
    public int getBallesRestantes(){return ballesRestantes;}
    public void resetActionsRestantes() {
        actionsRestantes = NB_ACTIONS;
    }

    public void executerAction() {
        if (!actionsPlanifiees.isEmpty()) {
            Action action = actionsPlanifiees.poll();
            action.executer();
        }
    }
    public ArrayList<Butin> getButins() {
        return butins;
    }
    @Override
    public void deplacer(Direction direction) {
        switch (direction) {
            case AVANT:
                if (positionWagon < Train.NB_WAGONS) {
                    positionWagon++;
                    System.out.println(nom+" a avancé");
                }
                break;
            case ARRIERE:
                if (positionWagon > 1) {

                    positionWagon--;
                    System.out.println(nom+" a reculé");

                }
                break;
            case HAUT:
                if (!surToit) {
                    surToit = true;
                    System.out.println(nom+" est monté");
                }
                break;
            case BAS:
                if (surToit) {
                    surToit = false;
                    System.out.println(nom+" est descendu");
                }
                break;
        }
    }
}