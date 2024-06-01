import javax.swing.*;
import java.awt.*;

// Classe pour afficher le gagnant et ses butins
class FenetreGagnant extends JFrame {
    public FenetreGagnant(Bandit gagnant) {
        setTitle("Gagnant du jeu");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());
        setSize(300, 200);
        setLocationRelativeTo(null);

        // Création du texte à afficher
        StringBuilder contenu = new StringBuilder();
        contenu.append("<html>Le gagnant est : ").append(gagnant.getNom()).append("<br/>Butins :<br/>");
        for (Butin butin : gagnant.getButins()) {
            contenu.append(butin.getClass().getSimpleName()).append(" - Valeur : ").append(butin.getValeur()).append("<br/>");
        }
        contenu.append("</html>");

        // Ajout du texte à la fenêtre
        JLabel labelGagnant = new JLabel(contenu.toString());
        add(labelGagnant, BorderLayout.CENTER);

        // Rendre la fenêtre visible
        setVisible(true);
    }
}

