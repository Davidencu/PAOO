#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>


// Playlist: gestioneaza un tablou dinamic de ID-uri de piese

class Playlist {
public:
    Playlist(std::size_t count, int defaultTrackId)
        : count_(count), tracks_(new int[count]) {
        std::fill(tracks_, tracks_ + count_, defaultTrackId);
    }

    // Deep copy (folosita si ca Strategia 3 la Item 14)
    Playlist(const Playlist& rhs)
        : count_(rhs.count_), tracks_(new int[rhs.count_]) {
        std::copy(rhs.tracks_, rhs.tracks_ + count_, tracks_);
    }

    // ITEM 10: returneaza referinta la *this (permite p1 = p2 = p3)
    // ITEM 11: copy-before-delete (sigur la p1 = p1 si la exceptii din new[])
    Playlist& operator=(const Playlist& rhs) {
        int* newTracks = new int[rhs.count_];
        std::copy(rhs.tracks_, rhs.tracks_ + rhs.count_, newTracks);
        delete[] tracks_;
        tracks_ = newTracks;
        count_ = rhs.count_;
        return *this;
    }

    // ITEM 10: aceeasi regula pentru += (adauga o piesa in playlist)
    Playlist& operator+=(int trackId) {
        int* newTracks = new int[count_ + 1];
        std::copy(tracks_, tracks_ + count_, newTracks);
        newTracks[count_] = trackId;
        delete[] tracks_;
        tracks_ = newTracks;
        ++count_;
        return *this;
    }

    virtual ~Playlist() { delete[] tracks_; }

    std::size_t count() const { return count_; }
    int track(std::size_t i) const { return tracks_[i]; }
    const void* bufferAddr() const { return tracks_; }

private:
    std::size_t count_;
    int* tracks_;
};


// ITEM 12: Copierea tuturor partilor obiectului (baza + derivata)

class NamedPlaylist : public Playlist {
public:
    NamedPlaylist(std::string title, std::size_t count, int defaultTrackId)
        : Playlist(count, defaultTrackId), title_(std::move(title)) {}

    NamedPlaylist(const NamedPlaylist& rhs)
        : Playlist(rhs),          // obligatoriu: copiaza partea de baza
          title_(rhs.title_) {}

    NamedPlaylist& operator=(const NamedPlaylist& rhs) {
        Playlist::operator=(rhs); // obligatoriu: atribuie partea de baza
        title_ = rhs.title_;
        return *this;
    }

    void print() const {
        std::cout << "  \"" << title_ << "\": " << count() << " piese, prima=#"
                  << track(0) << ", addr=" << bufferAddr() << "\n";
    }

private:
    std::string title_;
};


// ITEM 13: Functii helper pentru demonstratia RAII cu FILE*

void writeLog(bool fail) {
    // Custom deleter apeleaza automat std::fclose la iesirea din scope
    std::unique_ptr<std::FILE, decltype(&std::fclose)> file(
        std::tmpfile(), &std::fclose);

    if (file) {
        std::fputs("Log entry\n", file.get());
    }
    if (fail) {
        throw std::runtime_error("eroare la scriere");
    }
}


// ITEM 14: Strategii de copiere in clase RAII (conexiune la baza de date)

struct DbConnection {
    int id;
    void open()  const { std::cout << "  [DB #" << id << "] conectat\n"; }
    void close() const { std::cout << "  [DB #" << id << "] deconectat\n"; }
};

// 1) Interzicem copierea (o conexiune unica nu trebuie duplicata)
class UniqueDbSession {
public:
    explicit UniqueDbSession(DbConnection* conn) : conn_(conn) { conn_->open(); }
    ~UniqueDbSession() { conn_->close(); }

    UniqueDbSession(const UniqueDbSession&) = delete;
    UniqueDbSession& operator=(const UniqueDbSession&) = delete;

private:
    DbConnection* conn_;
};

// 2) Reference counting: shared_ptr cu deleter propriu (close in loc de delete)
class SharedDbSession {
public:
    explicit SharedDbSession(DbConnection* conn)
        : conn_(conn, [](DbConnection* c) { c->close(); }) {
        conn->open();
    }
    long owners() const { return conn_.use_count(); }

private:
    std::shared_ptr<DbConnection> conn_;
};

//Functia main

int main() {
    std::cout << "=== ITEM 10: operator= si += returneaza *this ===\n";
    {
        Playlist a(1, 10), b(1, 20), c(1, 30);
        a = b = c;
        (a += 40) += 50; // daca += returna prin valoare, #50 se pierdea pe o copie
        std::cout << "  a are " << a.count() << " piese: #" << a.track(0)
                  << ", #" << a.track(1) << ", #" << a.track(2) << "\n";
        std::cout << "  &(a = b) == &a ? " << std::boolalpha << (&(a = b) == &a) << "\n";
    }

    std::cout << "\n=== ITEM 11: autoatribuirea (a = a) ===\n";
    {
        Playlist a(2, 77);
        const void* before = a.bufferAddr();
        a = a;
        std::cout << "  inainte: " << before << ", dupa a = a: " << a.bufferAddr()
                  << " (piese intacte: " << a.count() << ")\n";
    }

    std::cout << "\n=== ITEM 12: copierea tuturor partilor ===\n";
    {
        NamedPlaylist orig("Synthwave", 2, 101);
        NamedPlaylist copy(orig);          // copy ctor: baza + titlu
        NamedPlaylist other("Gol", 1, 0);
        other = orig;                      // operator=: baza + titlu

        orig.print();
        copy.print();
        other.print();
    }

    std::cout << "\n=== ITEM 13: obiecte care gestioneaza resurse (RAII) ===\n";
    {
        writeLog(false);
        std::cout << "  iesire normala: fisier inchis automat\n";

        try {
            writeLog(true);
        } catch (const std::exception& e) {
            std::cout << "  exceptie prinsa (" << e.what() << "): fisier inchis automat\n";
        }

        // Transfer de proprietate cu unique_ptr (Strategia 4 din Item 14)
        std::unique_ptr<Playlist> p1 = std::make_unique<Playlist>(2, 10);
        // std::unique_ptr<Playlist> p2 = p1; // EROARE: copy ctor este sters
        std::unique_ptr<Playlist> p2 = std::move(p1);
        std::cout << "  dupa std::move: p1=" << (p1 ? "valid" : "null")
                  << ", p2=" << (p2 ? "valid" : "null") << "\n";
    }

    std::cout << "\n=== ITEM 14: strategii de copiere in clase RAII ===\n";
    {
        DbConnection conn1{1}, conn2{2};

        std::cout << "  1) Copiere interzisa (= delete):\n";
        {
            UniqueDbSession s1(&conn1);
            // UniqueDbSession s2(s1); // EROARE: copy ctor sters
        }

        std::cout << "  2) Contor de referinte (shared_ptr + custom deleter):\n";
        {
            SharedDbSession s1(&conn2);
            {
                SharedDbSession s2 = s1;
                std::cout << "     proprietari activi = " << s1.owners() << "\n";
            }
            std::cout << "     proprietari dupa iesirea copiei = " << s1.owners() << "\n";
        }

        std::cout << "  3) Copie adanca (deep copy): Playlist\n";
        {
            Playlist p1(2, 5);
            Playlist p2(p1);
            std::cout << "     p1.addr=" << p1.bufferAddr()
                      << ", p2.addr=" << p2.bufferAddr() << " (buffere separate)\n";
        }
    }

    return 0;
}