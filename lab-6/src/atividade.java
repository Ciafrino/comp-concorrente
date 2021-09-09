import java.util.Random;
import java.util.Scanner;

public class atividade {

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int N; // # de elementos
        final int nthreads; // # de threads

        System.out.println("Digite a quantidade de valores e threads desejada:");
        N = sc.nextInt();
        nthreads = sc.nextInt();

        Thread[] tid = new Thread[N];

        // Vamos construir nosso array agora
        // https://docs.oracle.com/javase/8/docs/api/java/util/Random.html
        Random rng = new Random();
        rng.setSeed(48);

        int[] values = new int[N];
        for (int i = 0; i < N; ++i) {
            values[i] = rng.nextInt();
        }

        // Vamos medir os tempos de execucao de ambos algoritmos em nanosegundos
        // https://stackoverflow.com/questions/3382954/measure-execution-time-for-a-java-method

        // inicio do timer concorrente
        double start_conc_timer = System.nanoTime();

        // criaçao de threads e inicio da contagem concorrente
        for (int id = 0; id < nthreads; ++id) {
            tid[id] = new Thread(new counter(values, id, nthreads));
        }

        for (int id = 0; id < nthreads; ++id) {
            tid[id].start();
        }

        for (int id = 0; id < nthreads; ++id) {
            try {
                tid[id].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        // fim do timer concorrente
        double end_conc_timer = System.nanoTime();
        // tempo de execucao final da parte concorrente
        double conc_timer = end_conc_timer - start_conc_timer;

        // inicio do timer sequencial
        double start_seq_timer = System.nanoTime();

        // versao sequencial
        int cnt = 0;
        for (int x : values) {
            if ((x & 1) == 0) ++cnt;
        }

        // fim do timer sequencial
        double end_seq_timer = System.nanoTime();
        // tempo de execucao final da parte sequencial
        double seq_timer = end_seq_timer - start_seq_timer;

        // garante que contamos contamos corretamente
        assert(counter.get_cnt() == cnt); // erro de execuçao se o teste falhar

        System.out.println("Passou no teste!");

        // tempo de execucao de ambas as partes
        System.out.println("Concorrente: " + conc_timer);
        System.out.println("Sequencial: " + seq_timer);
    }
}
