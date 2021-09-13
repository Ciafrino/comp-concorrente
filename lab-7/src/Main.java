public class Main {
    public static int L = 2;
    public static int E = 2;
    public static int LE = 2;

    public static int delayL = 48;
    public static int delayE = 48;
    public static int delayLE = 48;

    public static int NL = 10;
    public static int NE = 10;
    public static int NLE = 10;


    public static void main(String[] args) {
        Monit monitor = new Monit();

        Leitora[] aL = new Leitora[L];
        for (int id = 0; id < L; ++id) {
            aL[id] = new Leitora(id, NL, delayL, monitor);
            aL[id].start();
        }

        Escritora[] aE = new Escritora[E];
        for (int id = 0; id < E; ++id) {
            aE[id] = new Escritora(id, NE, delayE, monitor);
            aE[id].start();
        }

        LeitoraEscritora[] aLE = new LeitoraEscritora[LE];
        for (int id = 0; id < LE; ++id) {
            aLE[id] = new LeitoraEscritora(id, NLE, delayLE, monitor);
            aLE[id].start();
        }

    }
}
