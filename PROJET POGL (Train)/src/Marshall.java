
import java.util.Random;
import java.util.Queue;
import java.util.LinkedList;
public class Marshall extends individu {
    private final double NERVOSITE_MARSHALL; //proba de bouger a chaque action
    private final Queue<Action> actionsPlanifiees; //"pile" de ses actions dans l'ordre
    public Marshall(double nerv,int nbAction) {

        super("Marshall", 1, false,nbAction);
        NERVOSITE_MARSHALL=nerv;
        actionsPlanifiees=new LinkedList<>();
    }

    public void planifierAction(Action action) {
        actionsPlanifiees.add(action);
    }

    public boolean[] executerAction(Train train) {
        if (!actionsPlanifiees.isEmpty()) {
            Action action = actionsPlanifiees.poll();
            action.executer();
            return train.verifierRencontreMarshall(positionWagon);
        }
        return new boolean[train.getBandits().length];

    }
    @Override
    public void deplacer(Direction direction){
        switch (direction) {
            case AVANT:
                if (this.positionWagon < Train.NB_WAGONS) {
                    positionWagon++;
                    System.out.println("Le Marshall avance au wagon " + this.positionWagon);
                }
                break;
            case ARRIERE:
                if (this.positionWagon > 1) {
                    positionWagon--;
                    System.out.println("Le Marshall recule au wagon " + this.positionWagon);
                }
                break;
            default:
                // Le Marshall ne se déplace pas verticalement, donc HAUT et BAS ne font rien
                System.out.println("Le Marshall reste sur place.");
                break;
        }
    }

    public Direction planifierAction() {
        Random rand = new Random();
        // La nervosité détermine si le Marshall décide de bouger ou non
        if (rand.nextDouble() <= NERVOSITE_MARSHALL) {
            // Déterminer aléatoirement la direction du déplacement si aucun direction n'est donnée
            Direction direction = rand.nextBoolean() ? Direction.AVANT : Direction.ARRIERE;

            // Déplacement du Marshall basé sur la direction choisie
            switch (direction) {
                case AVANT:
                    if (this.positionWagon < Train.NB_WAGONS) { //si il peut avancer dans la direction choisie, sinon il va dans la direction opposée
                        planifierAction(new ActionAvancer(this));
                        return Direction.AVANT;
                    }else{
                        planifierAction(new ActionReculer(this));
                        return Direction.ARRIERE;
                    }
                case ARRIERE:
                    if (this.positionWagon > 1) { //même chose que ci-dessus
                        planifierAction(new ActionReculer(this));
                        return Direction.AVANT;
                    }else{
                        planifierAction(new ActionAvancer(this));
                        return Direction.ARRIERE;
                    }
                default:
                    // Le Marshall ne se déplace pas verticalement, donc HAUT et BAS ne font rien
                    planifierAction(new ActionStatique(this));
                    return null;
            }
        } else {
            planifierAction(new ActionStatique(this));
        }
        return null;
    }
}
