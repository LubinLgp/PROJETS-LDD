// Braquer.java
import java.util.ArrayList;
import java.util.Random;
// Braquer.java
class Braquer extends Action {
    private final Train train;

    public Braquer(individu cible, Train train) {
        super(cible);
        this.train = train;
    }

    @Override
    public void executer() {
        Bandit bandit = (Bandit) cible;
        // Vérifier si le bandit est à l'intérieur du wagon
        if (!bandit.isSurToit()) {
            Wagon wagon = train.getWagons()[bandit.getPositionWagon() - 1];
            ArrayList<Butin> butins = wagon.getButins();

            if (!butins.isEmpty()) {
                Random rand = new Random();
                int index = rand.nextInt(butins.size());
                Butin butin = butins.remove(index); // Retirer le butin aléatoirement du wagon
                if(butin instanceof MontreDeLuxe){
                    ((Bandit) cible).decrementerBallesRestantes();
                }
                bandit.getButins().add(butin); // Ajouter le butin au bandit qui braque
                bandit.notifyObservers();
            }
        }
    }
}
