public abstract class individu extends observable {
    protected String nom;
    protected int positionWagon;
    protected boolean surToit;
    protected static int NB_ACTIONS;


    public individu(String nom, int positionWagon, boolean surToit, int nbAction) {
        this.nom = nom;
        this.positionWagon = positionWagon;
        this.surToit = surToit;
        NB_ACTIONS=nbAction;
    }

    public String getNom() { return nom; }
    public int getPositionWagon() { return positionWagon; }
    public boolean isSurToit() { return surToit; }

    public abstract void deplacer(Direction direction);
}