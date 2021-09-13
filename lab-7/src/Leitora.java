// esboço criado com base no codigo feito durante a aula assincrona
public class Leitora extends Thread {
    private int N;
    private int delay;
    private Monit monitor;
    private int id;

    public Leitora(int id, int N, int delayTime, Monit monitor) {
        this.id = id;
        this.N = N;
        this.delay = delayTime;
        this.monitor = monitor;
    }

    // teste de primalidade ingenuo O(sqrt(val))
    public int is_prime(int val) {
        if (val < 2) return 0;
        for (int d = 2; d*d <= val; ++d) {
            if (val % d == 0) return 0;
        }
        return 1;
    }

    public void run() {
        for (int i = 0; i < this.N; ++i) {
            int id = this.id;
            this.monitor.EntraLeitor(id); // entra leitor

            int val = this.monitor.get_val(); // pega valor atual
            System.out.println("Leitor " + id + "  ---  " + val + ((is_prime(val) == 1) ? " eh primo" : " nao eh primo"));

            this.monitor.SaiLeitor(id); // sai leitor

            try {
                sleep(this.delay);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
