#pragma once
#include <cassert>
#include <map>

#include <kiraz/Node.h>

#include <lexer.hpp>
#include <unordered_set> 

enum class ScopeType {
    Module,
    Class,
    Func,
    Method,
};

struct Scope {
    using SymTab = std::map<std::string, Node::Ptr>;

    Scope(const std::map<std::string, Node::Ptr> &map, ScopeType stype, Node::Ptr s)
            : symbols(map), scope_type(stype), stmt(s) {}

    SymTab symbols;
    ScopeType scope_type;
    Node::Ptr stmt;

    auto find(const std::string &s) { return symbols.find(s); }
    auto find(const std::string &s) const { return symbols.find(s); }
    auto end() { return symbols.end(); }
    auto end() const { return symbols.end(); }

    decltype(auto) operator[](const std::string &s) { return (symbols[s]); }
    Node::SymTabEntry get_symbol(const std::string &name) const {
        auto iter = symbols.find(name);
        if (iter == symbols.end()) {
            return name;
        }
        return {name, iter->second};
    }

      void add_symbol(const std::string &name, Node::Ptr m) {
        assert(!name.empty());
        symbols[name] = m;
    }
};

class SymbolTable {
public:
    struct ScopeRef {
        ScopeRef(SymbolTable &s) : symtab(s) {}
        ScopeRef(const ScopeRef &) = delete;
        ScopeRef(ScopeRef &&) = default;
        ScopeRef &operator=(const ScopeRef &) = delete;

        ~ScopeRef() { symtab.exit_scope(); }

        SymbolTable &symtab;
    };

    friend class ScopeRef;

    explicit SymbolTable();
    explicit SymbolTable(ScopeType);

    virtual ~SymbolTable();

    bool is_builtin_keyword(const std::string& name) const {
        static const std::unordered_set<std::string> builtins = {
            "and", "or", "not"
        };
        return builtins.find(name) != builtins.end();
    }

    void add_builtin_keywords() {
        m_symbols.back()->add_symbol("and", nullptr);
        m_symbols.back()->add_symbol("or", nullptr);
        m_symbols.back()->add_symbol("not", nullptr);

        m_symbols.back()->add_symbol("Integer64", nullptr);
        m_symbols.back()->add_symbol("String", nullptr);
        m_symbols.back()->add_symbol("Void", nullptr);
        m_symbols.back()->add_symbol("Class", nullptr);
        m_symbols.back()->add_symbol("Module", nullptr);
        m_symbols.back()->add_symbol("Integer64", nullptr);
        m_symbols.back()->add_symbol("true", nullptr);
        m_symbols.back()->add_symbol("false", nullptr);

    }

    Node::Ptr add_symbol(const std::string &name, Node::Ptr m) {
        assert(! name.empty());
        (*m_symbols.back())[name] = m;
        return m;
    }

    Node::SymTabEntry get_symbol(const std::string &name) const {
        return m_symbols.back()->get_symbol(name);
    }

    const auto &get_symbols() const { return m_symbols.back()->symbols; }

    ScopeRef enter_scope(ScopeType scope_type, Node::Ptr stmt) {
        assert(stmt->get_cur_symtab() == m_symbols.back());
        m_symbols.emplace_back(
                std::make_shared<Scope>(m_symbols.back()->symbols, scope_type, stmt));
        assert(m_symbols.size() > 1);
        return ScopeRef(*this);
    }

    auto get_cur_symtab() { return m_symbols.back(); }
    auto get_cur_symtab() const { return m_symbols.back(); }
    auto get_scope_type() const { return m_symbols.back()->scope_type; }
    auto get_scope_stmt() const { return m_symbols.back()->stmt; }

    static auto get_module_io() { return s_module_io; }

private:
    void exit_scope() { m_symbols.pop_back(); }

    std::vector<std::shared_ptr<Scope>> m_symbols;

    static Node::Ptr s_module_ki;
    static Node::Ptr s_module_io;
};

class WasmContext {
    struct Streams {
        std::stringstream locals;
        std::stringstream body;
    };

public:
    WasmContext() : m_streams(1) {}

    struct Coords {
        Coords(uint32_t o = 0, uint32_t l = 0) : offset(o), length(l) {}
        uint32_t offset;
        uint32_t length;
    };

    const auto &get_memory() const { return m_memory; }
    std::string_view get_memory_view() const {
        return {reinterpret_cast<const char *>(m_memory.data()), m_memory.size()};
    }

    /**
     * @brief add_to_memory: Adds the given string to static memory
     * @param s: String to add
     * @return Memory coordinates of the given string
     */
    Coords add_to_memory(const std::string &s);

    /**
     * @brief add_to_memory: Adds the given u32 to static memory
     * @param u: u32 to add
     * @return Memory coordinates of the given u32
     */
    Coords add_to_memory(uint32_t u);

    auto &body() { return m_streams.back().body; }
    auto &body() const { return m_streams.back().body; }
    auto &locals() { return m_streams.back().locals; }

    void push() { m_streams.emplace_back(); }
    void pop() {
        assert(m_streams.size() >= 2);
        {
            auto iter = m_streams.rbegin();
            auto &source = *iter;
            auto &target = *std::next(iter);
            target.body << source.locals.str();
            target.body << source.body.str();
        }
        m_streams.pop_back();
        assert(m_streams.size() > 0 || m_streams.back().locals.str().empty());
    }

private:
    std::vector<unsigned char> m_memory;
    std::vector<Streams> m_streams;
};

class Compiler {
public:
    static Compiler *current() { return s_current; }
    Compiler();

    int compile_file(const std::string &file_name);
    int compile_string(const std::string &str);
    Node::Ptr compile_module(const std::string &str);

    static void reset_parser();
    void reset();
    void set_error(const std::string &str) { m_error = str; }
    const auto &get_error() const { return m_error; }
    const auto &get_wasm_ctx() const { return m_ctx; }

    ~Compiler();

protected:
    int compile(Node::Ptr root);

private:
    YY_BUFFER_STATE buffer = nullptr;
    std::string m_error;
    WasmContext m_ctx;
    static Compiler *s_current;
};
