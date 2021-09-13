// esboço criado com base no codigo feito durante a aula assincrona
public class Monit {
    public int val; // variavel inteira que vai ser alterada pelas threads

    private int leit;
    private int escr;

    Monit() {
        this.leit = 0;
        this.escr = 0;
        this.val = 0;
    }


    public synchronized void EntraLeitor(int id) {
        try {
            while (this.escr > 0) {
                System.out.println("le.leitorBloqueado(" + id + ")");
                wait();
            }
            this.leit += 1;
            System.out.println("le.leitorLendo(" + id + ")");
        } catch (InterruptedException e) {}
    }

    public synchronized void SaiLeitor(int id) {
        this.leit -= 1;
        if (this.leit == 0) this.notify();
        System.out.println("le.leitorSaindo(" + id + ")");
    }

    public synchronized void EntraLE(int id) {
        try {
            while (this.escr > 0 || this.leit > 0) {
                System.out.println("le.LeitorEscritorBloqueado(" + id + ")");
                wait();
            }
            this.escr += 1;
            System.out.println("le.LeitorEscritorEscrevendo(" + id + ")");
        } catch (InterruptedException e) {}
    }

    public synchronized void SaiLE(int id) {
        this.escr -= 1;
        notifyAll();
        System.out.println("le.LeitorEscritorSaindo(" + id + ")");
    }

    public synchronized void EntraEscritor(int id) {
        try {
            while (this.escr > 0 || this.leit > 0) {
                System.out.println("le.escritorBloqueado(" + id + ")");
                wait();
            }
            this.escr += 1;
            System.out.println("le.escritorEscrevendo(" + id + ")");
        } catch (InterruptedException e) {}
    }

    public synchronized void SaiEscritor(int id) {
        this.escr -= 1;
        notifyAll();
        System.out.println("le.escritorSaindo(" + id + ")");
    }

    // funçao para alterar valor da nossa variavel
    public synchronized void set_val(int new_val) { this.val = new_val; }

    // funçao para ver valor atual da nossa variavel
    public synchronized int get_val() { return this.val; }
}
