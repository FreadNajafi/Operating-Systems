
package coe628.lab9;

/**
 *
 * @author a4najafi
 */

import java.util.logging.Level;
import java.util.logging.Logger;

public class DiningPhilosophers {
    private static final int NUM_PHILOSOPHERS = 5; 
    private static final Semaphore[] forks = new Semaphore[NUM_PHILOSOPHERS]; 
    public static Philosopher[] philosophers = new Philosopher[NUM_PHILOSOPHERS];

    enum PhilosopherState { 
        THINKING,
        HUNGRY,
        EATING,
    }

    static int completedPhilosophers = 0;
    public static PhilosopherState[] state = new PhilosopherState[NUM_PHILOSOPHERS]; 

    private static Semaphore mutex = new Semaphore(1); 

    private static int RIGHT(int i) {
        return (i + 1) % NUM_PHILOSOPHERS;
    }

    private static int LEFT(int i) {
        return (i == 0) ? NUM_PHILOSOPHERS - 1 : i - 1;
    }

    public static void main(String args[]) {
        for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
            forks[i] = new Semaphore(0);
            philosophers[i] = new Philosopher();
        }

        for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
            philosophers[i].start();
        }
    }

    public static class Philosopher extends Thread {
        private static int idCounter = 0; 
        private int id; 
        private boolean completed;

        public Philosopher() {
            this.id = idCounter;
            idCounter++;
            this.completed = false;
        }

        private void test(int i) {
            if (state[i] == PhilosopherState.HUNGRY &&
                    state[LEFT(i)] != PhilosopherState.EATING &&
                    state[RIGHT(i)] != PhilosopherState.EATING) {
                state[i] = PhilosopherState.EATING;
                forks[i].up();
            }
        }

        private void think() throws InterruptedException {
            Thread.sleep((long) (Math.random() * 1000));
        }

        private void getForks() throws InterruptedException {
            mutex.down(); 
            state[id] = PhilosopherState.HUNGRY; 
            test(id); 
            mutex.up(); 
            System.out.println("Philosopher " + (id + 1) + " is waiting for Fork " + (RIGHT(id) + 1));
            forks[id].down();
            System.out.println("Fork " + (id + 1) + " taken by Philosopher " + (id + 1));
            System.out.println("Fork " + (RIGHT(id) + 1) + " taken by Philosopher " + (id + 1));
        }

        private void eat() throws InterruptedException {
            Thread.sleep((long) (Math.random() * 1000));
        }

        private void putForks() throws InterruptedException {
            mutex.down(); 
            state[id] = PhilosopherState.THINKING; 
            test(LEFT(id)); 
            test(RIGHT(id)); 
            mutex.up(); 
            completedPhilosophers++; 
            this.completed = true;
        }

        private boolean isCompleted() {
            return this.completed;
        }
        
        @Override
        public void run() {
            while (!isCompleted()) {
                try {
                    think();
                    getForks();
                    eat();
                    putForks();
                } catch (InterruptedException e) {
                    Logger.getLogger(DiningPhilosophers.class.getName()).log(Level.SEVERE, null, e);
                    e.printStackTrace();
                }

                for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
                    if (philosophers[i].isCompleted()) {
                        System.out.println("Philosopher " + (philosophers[i].id + 1) + " completed his dinner");
                    }
                }
                System.out.println("Philosopher " + (id + 1) + " released fork " + (id + 1) + " and " + (RIGHT(id) + 1));
                System.out.println("Num of philosophers completed dinner are now " + completedPhilosophers);
            }
        }
    }
}
