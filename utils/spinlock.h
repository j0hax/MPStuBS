class Spinlock {

private:
    bool free = true;

public:
    Spinlock() {

    }

    /*!
     * @brief Betritt den gesperrten Abschnitt. Ist dieser besetzt, so wird solange aktiv gewartet, bis er betreten werden kann.
     * 
     */
       void lock() {
        while (!free);
        free = false;
    }

    /*!
     * @brief Gibt den gesperrten Abschnitt wieder frei.
     * 
     */
    void unlock() {
        free = true;
    }
};