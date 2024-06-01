import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.util.Iterator;
import java.util.Random;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;


public class VueCommandes extends JPanel {
    private final Bandit[] bandits;
    private final Marshall marshall;
    private final VueTrain vueTrain;
    private final VueBandit vueBandit;
    private final VueButins vueButins;
    private final VueMarshall vueMarshall;
    private final VueCouleur vueCouleur;
    private final Train train;
    private int posMontre=2; //la personne avec la montre est au début dans le deuxième wagon (=premiere classe)
    private JLabel statusLabel; //status du jeu
    private JLabel statusWin; //Jlabel pour gagner avec la montre
    private JLabel statusWinSomme; //Jlabel pour gagner avec la somme
    private JButton monterButton;
    private JButton descendreButton;
    private JButton gaucheButton;
    private JButton droiteButton;
    private JButton braquerButton;
    private JButton tirerHautButton;
    private JButton tirerBasButton;
    private JButton tirerAvantButton;
    private JButton tirerArriereButton;

    private final AtomicInteger joueurActuelIndex = new AtomicInteger(0); //indice du joueur qui joue actuellement
    private final AtomicInteger etapeAction = new AtomicInteger(0); //numéro de l'étape actuelle
    private boolean enPlanification = true; //true = phase de planication, false=phase d'action
    //Les deux variables suivantes servent à s'assurer que le bandit peut se deplacer dans une direction, avant d'appuyer sur action
    private final AtomicBoolean[] surLeToit; //sauvegarde avant d'appuyer sur action si chaque bandit (dans chaque position du tableau), est sur le toit ou pas
    private final AtomicInteger[] positions; //sauvegarde avant d'appuyer sur action le numéro de wagon de chaque bandit



    public VueCommandes(Bandit[] bandits, VueTrain vueTrain, VueBandit vueBandit, VueButins vueButins, VueMarshall vueMarshall, Marshall marshall, Train train, VueCouleur vueCouleur) {
        setOpaque(false);
        this.bandits = bandits;
        this.surLeToit = new AtomicBoolean[bandits.length];
        this.positions = new AtomicInteger[bandits.length];
        for (int i = 0; i < bandits.length; i++) {
            this.surLeToit[i] = new AtomicBoolean(bandits[i].isSurToit());
            this.positions[i] = new AtomicInteger(bandits[i].getPositionWagon());
        }
        this.vueTrain = vueTrain;
        this.vueBandit = vueBandit;
        this.vueButins = vueButins;
        this.vueMarshall = vueMarshall;
        this.marshall = marshall;
        this.train = train;
        this.vueCouleur=vueCouleur;
        AtomicInteger posMarshall = new AtomicInteger(marshall.getPositionWagon());

        setLayout(new GridBagLayout());
        setupUIComponents(posMarshall);
        updateStatus();
    }

    private void setupUIComponents(AtomicInteger posMarshall) { //place les boutons
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.insets=new Insets(2,2,2,2);

        statusLabel = new JLabel();
        statusWin = new JLabel();
        statusWinSomme = new JLabel();
        gbc.gridx=4;
        gbc.gridy=0;
        add(statusLabel, gbc);
        gbc.gridy=2;
        add(statusWin,gbc);
        gbc.gridy=1;
        add(statusWinSomme,gbc);

        monterButton = new JButton("Monter");
        monterButton.addActionListener(e->planifierAction(e,posMarshall));

        descendreButton = new JButton("Descendre");
        descendreButton.addActionListener(e->planifierAction(e,posMarshall));

        gaucheButton = new JButton("Gauche");
        gaucheButton.addActionListener(e->planifierAction(e,posMarshall));

        droiteButton = new JButton("Droite");
        droiteButton.addActionListener(e->planifierAction(e,posMarshall));

        braquerButton = new JButton("Braquer");
        braquerButton.addActionListener(e->planifierAction(e,posMarshall));
        tirerHautButton = new JButton("Tirer haut");
        tirerBasButton = new JButton("Tirer bas");
        tirerAvantButton = new JButton("Tirer avant");
        tirerArriereButton = new JButton("Tirer arrière");

        tirerHautButton.addActionListener(e -> planifierAction(e,posMarshall));
        tirerBasButton.addActionListener(e -> planifierAction(e,posMarshall));
        tirerAvantButton.addActionListener(e -> planifierAction(e,posMarshall));
        tirerArriereButton.addActionListener(e -> planifierAction(e,posMarshall));

        JButton actionButton = new JButton("Action");
        actionButton.addActionListener(this::executerActions);
        gbc.gridx=1;
        gbc.gridy=0;
        add(monterButton, gbc);
        gbc.gridx=1;
        gbc.gridy=2;
        add(descendreButton, gbc);
        gbc.gridx=0;
        gbc.gridy=1;
        add(gaucheButton, gbc);
        gbc.gridx=2;
        gbc.gridy=1;
        add(droiteButton, gbc);
        gbc.gridx=1;
        gbc.gridy=1;
        add(braquerButton, gbc);
        gbc.gridx=6;
        gbc.gridy=1;
        add(actionButton, gbc);
        gbc.gridx=6;
        gbc.gridy=0;
        add(tirerHautButton, gbc);
        gbc.gridx=6;
        gbc.gridy=2;
        add(tirerBasButton, gbc);
        gbc.gridx=5;
        gbc.gridy=1;
        add(tirerAvantButton, gbc);
        gbc.gridx=7;
        gbc.gridy=1;

        add(tirerArriereButton, gbc);
        gbc.gridwidth=9;
        gbc.gridheight=4;
        gbc.anchor=GridBagConstraints.WEST;

    }

    private void planifierAction(ActionEvent e,AtomicInteger posMarshall) {
        int indexActuel = joueurActuelIndex.get();

        Bandit actuel = bandits[indexActuel];
        AtomicBoolean toitActuel = surLeToit[indexActuel];
        AtomicInteger positionActuelle = positions[indexActuel];

        if (enPlanification) {
            Action action = null;

            if (e.getSource() == monterButton && !toitActuel.get()) {
                toitActuel.set(true); // Le bandit monte sur le toit
                action = new ActionMonter(actuel);
            } else if (e.getSource() == descendreButton && toitActuel.get()) {
                toitActuel.set(false); // Le bandit descend dans le wagon
                action = new ActionDescendre(actuel);
            } else if (e.getSource() == gaucheButton && positionActuelle.get() > 1) {
                positionActuelle.decrementAndGet(); // Le bandit se déplace vers le wagon précédent
                action = new ActionReculer(actuel);
            } else if (e.getSource() == droiteButton && positionActuelle.get() < Train.NB_WAGONS) {
                positionActuelle.incrementAndGet(); // Le bandit se déplace vers le wagon suivant
                action = new ActionAvancer(actuel);
            } else if (e.getSource() == braquerButton && !toitActuel.get()) {
                // Le bandit braque si et seulement s'il est à l'intérieur d'un wagon
                action = new ActionBraquer(actuel, train);
            }else if(e.getSource() == tirerArriereButton&& positionActuelle.get() < Train.NB_WAGONS){
                action = new ActionTir(actuel,Direction.ARRIERE,train);
            }else if(e.getSource() == tirerAvantButton&& positionActuelle.get()>1){
                action = new ActionTir(actuel,Direction.AVANT,train);
            }else if(e.getSource() == tirerHautButton){
                action = new ActionTir(actuel,Direction.HAUT,train);
            }else if(e.getSource() == tirerBasButton){
                action = new ActionTir(actuel,Direction.BAS,train);
            }

            if (action != null) {
                if(indexActuel==0){
                    Direction d = marshall.planifierAction();
                    if(d==Direction.AVANT){
                        posMarshall.set(posMarshall.get()-1);

                    }else if(d==Direction.ARRIERE){

                        posMarshall.set(posMarshall.get()+1);
                    }
                }
                actuel.planifierAction(action);
                actuel.decrementerActionsRestantes();
                if (actuel.getActionsRestantes() == 0 && indexActuel < bandits.length - 1) {
                    // Passer au prochain bandit pour la planification s'il reste des bandits
                    joueurActuelIndex.incrementAndGet();
                } else if (indexActuel == bandits.length - 1 && actuel.getActionsRestantes() == 0) {
                    // Dernier bandit a fini de planifier, passer à la phase d'exécution
                    enPlanification = false;
                }
                updateStatus(); // Mettre à jour le statut
            }
        }

    }


    private void executerActions(ActionEvent e) {


        boolean banditPossedeMontre=false;//true si la montre est détenue par un bandit, false sinon
        Bandit banditMontre=null; //bandit qui possède la montre
        if (!enPlanification && etapeAction.get() < Bandit.NB_ACTIONS) {//Si on est en phase d'action

            for (Bandit bandit : bandits) {// est ce que la montre est détenue par quelqu'un ?si oui qui ?
                bandit.executerAction();
                if(bandit.possedeMontre()){
                    banditPossedeMontre=true;
                    banditMontre=bandit;
                }

            }
            boolean[] rencontre = marshall.executerAction(train); //execute l'action du marshall et renvoie un tableau de boolean pour savoir quels bandits sont dans le même wagon que le marshall
            for(int i=0;i< rencontre.length;i++) {
                if (rencontre[i] && !surLeToit[i].get()) {
                    surLeToit[i].set(true); //change l'atomic boolean
                }
            }
            Random rand = new Random();
            if (rand.nextDouble() <= train.getNervositeMontre()) { //si la personne detenant la montre se deplace
                Random rand2 = new Random();
                MontreDeLuxe montreTrouvee = null;

                for (Wagon wagon : train.getWagons()) {
                    for (Butin butin : wagon.getButins()) {
                        if (butin instanceof MontreDeLuxe) {
                            montreTrouvee = (MontreDeLuxe) butin;
                            break;
                        }
                    }
                    if (montreTrouvee != null) break;
                }
                if(montreTrouvee != null) { //si la montre est encore dans le train
                    if (rand2.nextDouble() <= 0.5 && posMontre >= 2) { //pour que la montre avance

                        boolean montreRetiree = false;

                        for (Wagon wagon : train.getWagons()) {
                            Iterator<Butin> iterator = wagon.getButins().iterator(); //parcourt tous les butins
                            while (iterator.hasNext()) {
                                Butin butin = iterator.next();
                                if (butin.equals(montreTrouvee)) { // Si c'est la montre
                                    iterator.remove(); // Retirer la montre de la liste
                                    montreRetiree = true;
                                    break;
                                }
                            }
                            if (montreRetiree) break;
                        }
                        train.getWagons()[posMontre - 2].getButins().add(montreTrouvee);//ajoute la montre dans le nouveau wagon
                        posMontre--;
                    } else if (posMontre < train.getNB_WAGONS() - 1) {//la montre recule
                        boolean montreRetiree = false;

                        for (Wagon wagon : train.getWagons()) {
                            Iterator<Butin> iterator = wagon.getButins().iterator();
                            while (iterator.hasNext()) {
                                Butin butin = iterator.next();
                                if (butin.equals(montreTrouvee)) {
                                    iterator.remove();
                                    montreRetiree = true;
                                    break;
                                }
                            }
                            if (montreRetiree) break;
                        }
                        train.getWagons()[posMontre].getButins().add(montreTrouvee);
                        posMontre++;
                    }
                }
            }
            if(etapeAction.get()==Bandit.NB_ACTIONS-1  && banditPossedeMontre){//si le tour est fini, on verifie que personne n'a gagné et on augmente le compteur de tours avec la montre si un bandit la detient
                banditMontre.incrementeTour();
                verifierEtTerminerJeu(banditMontre);
            }if(etapeAction.get()==Bandit.NB_ACTIONS-1) { //sinon, on verifie si un bandit a réuni la somme necessaire pour gagner (Donc le gain avec la montre est prioritaire par rapport au gain avec la somme)
                for (Bandit b : train.getBandits()) {
                    verifierGainEtTerminerJeu(b);
                }
            }
            etapeAction.incrementAndGet();
            if (etapeAction.get() == individu.NB_ACTIONS) {//si la phase d'action est finie, on re switch
                enPlanification = true;
                resetActionsPlanification();
            }
            //On met toutes les vues à jour
            vueCouleur.repaint();
            vueTrain.repaint(); // Mettre à jour la vue du train
            vueBandit.repaint(); // Mettre à jour la vue du bandit
            vueMarshall.repaint();
            vueButins.updateButins(train.getBandits());
            vueButins.repaint();
            updateStatus();
        }
    }

    private void resetActionsPlanification() {
        for (Bandit bandit : bandits) {
            bandit.resetActionsRestantes();
        }
        etapeAction.set(0);
        joueurActuelIndex.set(0);
    }
    private int nombreTourWin(){
        for(Bandit bandit: bandits) {
            for(Butin b:bandit.getButins()){
                if(b instanceof MontreDeLuxe){
                    return bandit.getNbTourAvecMontre();
                }
            }
        }
        return 0;
    }
    private void updateStatus() {
        statusWin.setText("Nombre de tours avec montre : "+nombreTourWin());
        statusWinSomme.setText("   Somme pour gagner : "+(int) train.getSommeTotale());
        if (enPlanification) {
            if (joueurActuelIndex.get() < bandits.length) {
                Bandit actuel = bandits[joueurActuelIndex.get()];
                statusLabel.setText(" "+actuel.getNom() + " : " + actuel.getActionsRestantes() + "  actions restantes.");
            } else {
                statusLabel.setText("Prêt pour l'exécution des actions.");
            }
        } else {

            statusLabel.setText("Exécution des actions - Étape " + (etapeAction.get() + 1) + " de " + individu.NB_ACTIONS + ".");
        }
    }
    public void verifierEtTerminerJeu(Bandit bandit) {
        boolean test = bandit.getNbTourAvecMontre() == bandit.getNombreTourPourWin();
        if (test) {
            new FenetreGagnant(bandit);
        }
    }
    public void verifierGainEtTerminerJeu(Bandit bandit){
        boolean test=bandit.getSommeTotale()>=train.getSommeTotale();
        if(test){
            new FenetreGagnant(bandit);
        }
    }
}


