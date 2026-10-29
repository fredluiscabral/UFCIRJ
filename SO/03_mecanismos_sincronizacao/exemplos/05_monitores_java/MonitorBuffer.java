public class MonitorBuffer {
    static class Buffer {
        private int item;
        private boolean cheio = false;
        public synchronized void produzir(int valor) throws InterruptedException {
            while (cheio) wait();
            item = valor;
            cheio = true;
            System.out.println("Produziu " + valor);
            notifyAll();
        }
        public synchronized int consumir() throws InterruptedException {
            while (!cheio) wait();
            int resultado = item;
            cheio = false;
            System.out.println("Consumiu " + resultado);
            notifyAll();
            return resultado;
        }
    }
    public static void main(String[] args) throws InterruptedException {
        Buffer buffer = new Buffer();
        Thread produtor = new Thread(() -> {
            try {
                for (int i=1;i<=5;i++) buffer.produzir(i);
            } catch (InterruptedException e) { Thread.currentThread().interrupt(); }
        });
        Thread consumidor = new Thread(() -> {
            try {
                for (int i=1;i<=5;i++) buffer.consumir();
            } catch (InterruptedException e) { Thread.currentThread().interrupt(); }
        });
        produtor.start(); consumidor.start();
        produtor.join(); consumidor.join();
        System.out.println("Fim");
    }
}
