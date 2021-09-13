// terceira thread
public class LeitoraEscritora extends Thread {
    private int N;
    private int delay;
    private Monit monitor;
    private int id;

    public LeitoraEscritora(int id, int N, int delayTime, Monit monitor) {
        this.id = id;
        this.N = N;
        this.delay = delayTime;
        this.monitor = monitor;
    }

    public void run() {
        for (int i = 0; i < this.N; ++i) {
            int id = this.id;

            // entra LE
            this.monitor.EntraLE(id);

            int val = this.monitor.get_val(); // valor atual da variavel
            int new_val = (val << 1); // novo valor da variavel

            System.out.println("LeitorEscritor " + id + "  ---  " + val + ((val % 2 == 1) ? "  eh impar" : "  eh par"));
            System.out.println("LeitorEscritor " + id + " variavel modificada para " + new_val);

            this.monitor.set_val(new_val); // modifica variavel

            // Sai LE
            this.monitor.SaiLE(id);

            try {
                sleep(this.delay);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
