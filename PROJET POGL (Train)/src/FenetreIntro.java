import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Objects;
import javax.sound.sampled.*;

public class FenetreIntro extends JFrame {

    private final ImageIcon backgroundIcon = new ImageIcon(Objects.requireNonNull(getClass().getResource("/backgroundrules.png")));

    public FenetreIntro() {
        setTitle("Bienvenue dans Colt Express");
        setSize(700, 400);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        // Panel de fond avec image d'arrière-plan
        JPanel backgroundPanel = new JPanel(new BorderLayout()) {
            @Override
            protected void paintComponent(Graphics g) {
                super.paintComponent(g);
                g.drawImage(backgroundIcon.getImage(), 0, 0, getWidth(), getHeight(), this);
            }
        };
        setContentPane(backgroundPanel);
        playSound("src/intro.wav");
        // Zone de texte avec les règles
        JScrollPane scrollPane = getjScrollPane();
        scrollPane.setOpaque(false);
        scrollPane.getViewport().setOpaque(false);
        scrollPane.setBorder(BorderFactory.createEmptyBorder());

        // Panneau de boutons
        JPanel buttonPanel = new JPanel();
        buttonPanel.setOpaque(false);
        JButton btnCommencer = new JButton("Commencer");
        JButton btnQuitter = new JButton("Quitter");
        btnCommencer.addActionListener(e -> {
            dispose();
            JFrame frame = new JFrame();
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

            // Barre coulissante pour sélectionner le nombre de joueurs
            JSlider slider = new JSlider(1, 4, 1);
            slider.setMajorTickSpacing(1);
            slider.setPaintTicks(true);
            slider.setPaintLabels(true);

            JOptionPane.showMessageDialog(frame, slider, "Sélectionnez le nombre de joueurs", JOptionPane.QUESTION_MESSAGE);

            int nbJoueurs = slider.getValue();

            // Champs pour saisir les noms des joueurs
            JTextField[] nameFields = new JTextField[nbJoueurs];
            JPanel panel = new JPanel(new GridLayout(0, 1));
            for (int i = 0; i < nbJoueurs; i++) {
                nameFields[i] = new JTextField("Joueur " + (i + 1));
                panel.add(nameFields[i]);
            }

            JOptionPane.showMessageDialog(frame, panel, "Entrez les noms des joueurs", JOptionPane.QUESTION_MESSAGE);

            String[] playerNames = new String[nbJoueurs];
            for (int i = 0; i < nbJoueurs; i++) {
                playerNames[i] = nameFields[i].getText();
            }

            // Sélection de la difficulté
            String[] difficulties = {"Facile", "Moyen", "Difficile"};
            String difficulty = (String) JOptionPane.showInputDialog(frame, "Choisissez la difficulté :",
                    "Sélection de la difficulté", JOptionPane.QUESTION_MESSAGE, null, difficulties, difficulties[0]);

            // Affichage pour vérification
            System.out.println("Nombre de joueurs : " + nbJoueurs);
            for (String name : playerNames) {
                System.out.println("Nom du joueur : " + name);
            }
            System.out.println("Difficulté choisie : " + difficulty);

            frame.dispose(); // Ferme la fenêtre initiale maintenant que les données sont collectées
            if(difficulty==null){
                return;
            }

            // Création d'un objet Train
            Train train = new Train(nbJoueurs,playerNames,difficulty);
            ArrayList<Bandit> banditsList = new ArrayList<>(Arrays.asList(train.getBandits()));

            // Mélanger la liste des bandits pour un ordre de jeu aléatoire
            Collections.shuffle(banditsList);

            // Convertir la liste mélangée en tableau et l'assigner à train ou le manipuler directement si possible
            Bandit[] shuffledBandits = banditsList.toArray(new Bandit[0]);
            train.setBandits(shuffledBandits);
            Bandit[] bandits=train.getBandits();
            Marshall marshall=train.getMarshall();

            Vue V = new Vue(train, bandits,marshall);


            // Création d'une fenêtre pour afficher la vue du train
            JFrame frame2 = new JFrame("Colt Express");
            frame2.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            playSound("src/tchoutchou.wav");
            // Ajustement de la taille de la fenêtre en fonction du contenu
            frame2.pack();

            // Rendre la fenêtre visible
            frame2.setVisible(true);
        });

        btnQuitter.addActionListener(e -> System.exit(0));

        buttonPanel.add(btnCommencer);
        buttonPanel.add(btnQuitter);

        // Ajout des composants au backgroundPanel
        backgroundPanel.add(scrollPane, BorderLayout.CENTER);
        backgroundPanel.add(buttonPanel, BorderLayout.SOUTH);

        // Rendre la fenêtre visible
        setVisible(true);
    }

    private static JScrollPane getjScrollPane() {
        JTextArea textArea = new JTextArea();
        textArea.setText("""
                                                                  Bienvenue Dans Colt Express :

                L'objectif : récolter le plus gros des magots !!!

                Attraper un maximum de gain ou la montre de l'ambassadeur pour gagner la partie.
                Pour gagner la partie, ramassez la somme indiquée, ou conservez la montre pendant 3 tours de suite !
                Mais faites attention à vous ! Un Marshall vous cherche dans le train et fera tout pour vous arrêter.
                Enfin, NE FAITES CONFIANCE À PERSONNE, certains bandits peuvent vous tirer dessus pour vous voler la victoire.
                """);
        textArea.setEditable(false);
        textArea.setOpaque(false);
        textArea.setFont(new Font("Serif", Font.BOLD, 18));

        textArea.setWrapStyleWord(true);
        textArea.setLineWrap(true);

        // Défilement
        return new JScrollPane(textArea);
    }
    public void playSound(String filePath) {
        try {
            AudioSystem AudioSystem = null;
            AudioInputStream audioInputStream = javax.sound.sampled.AudioSystem.getAudioInputStream(new File(filePath).getAbsoluteFile());
            Clip clip = javax.sound.sampled.AudioSystem.getClip();
            clip.open(audioInputStream);
            clip.loop(Clip.LOOP_CONTINUOUSLY); // Configure le clip pour boucler indéfiniment
        } catch (UnsupportedAudioFileException | IOException | LineUnavailableException e) { e.printStackTrace(); } }

}
