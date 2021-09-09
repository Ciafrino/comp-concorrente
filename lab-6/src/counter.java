public class counter implements Runnable {
    public static int[] values;
    public static int thread_id;
    public static int nthreads;
    public static int len;
    public static int cnt = 0;

    public counter(int[] values, int thread_id, int nthreads) {
        this.values = values;
        this.thread_id = thread_id;
        this.nthreads = nthreads;
        this.len = values.length;
    }

    // https://docs.oracle.com/javase/tutorial/essential/concurrency/syncmeth.html
    public synchronized static void increment() {
        cnt += 1;
    }

    public synchronized static int get_cnt() {
        return cnt;
    }

    public void run() {
        for (int id = thread_id; id < len; id += nthreads) {
            if ((values[id] & 1) == 0) increment();
        }
    }
}
