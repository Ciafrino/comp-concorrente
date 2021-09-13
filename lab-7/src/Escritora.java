// esboço criado com base no codigo feito durante a aula assincrona
public class Escritora extends Thread {
    private int N;
    private int delay;
    private Monit monitor;
    private int id;

    public Escritora(int id, int N, int delayTime, Monit monitor) {
        this.id = id;
        this.N = N;
        this.delay = delayTime;
        this.monitor = monitor;
    }

    public void run() {
        for (int i = 0; i < this.N; ++i) {
            int id = this.id;
            this.monitor.EntraEscritor(id); // entra escritor

            System.out.println("Escritor " + id + "  modificado para  " + id);

            this.monitor.set_val(id); // modifica variavel para valor da thread

            this.monitor.SaiEscritor(id); // sai escritor

            try {
                sleep(this.delay);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}

