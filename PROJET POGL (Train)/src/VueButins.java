import javax.swing.*;
import java.awt.*;

public class VueButins extends JPanel {

    public VueButins(Bandit[] bandits) {
        setLayout(new GridLayout(1, bandits.length)); // Organisation horizontale des informations des bandits
        setOpaque(false);

        for (Bandit bandit : bandits) {
            add(createBanditInfoPanel(bandit)); // Créer et ajouter le panel d'info pour chaque bandit
        }
    }

    private JPanel createBanditInfoPanel(Bandit bandit) {
        JPanel panelBandit = new JPanel();
        panelBandit.setLayout(new BoxLayout(panelBandit, BoxLayout.Y_AXIS));
        panelBandit.setOpaque(false);

        // Nom du bandit
        JLabel labelNom = new JLabel(bandit.getNom() + " possède:");
        panelBandit.add(labelNom);

        // Balles restantes
        JLabel labelBalles = new JLabel("Balles restantes : " + bandit.getBallesRestantes());
        panelBandit.add(labelBalles);

        // Montant total récolté
        JLabel labelTotal = new JLabel("Montant total récolté : " + bandit.getSommeTotale());
        panelBandit.add(labelTotal);

        // Liste des butins
        if (bandit.getButins().isEmpty()) {
            panelBandit.add(new JLabel("Aucun butin"));
        } else {
            for (Butin butin : bandit.getButins()) {
                panelBandit.add(new JLabel(butin.getClass().getSimpleName() + " - Valeur : " + butin.getValeur()));
            }
        }

        return panelBandit;
    }

    public void updateButins(Bandit[] bandits) {
        removeAll(); // Effacer tous les composants actuels
        for (Bandit bandit : bandits) {
            add(createBanditInfoPanel(bandit)); // Recréer et ajouter les informations pour chaque bandit
        }
        revalidate(); // Redéfinir la disposition des composants
        repaint(); // Redessiner le panel
    }
}
