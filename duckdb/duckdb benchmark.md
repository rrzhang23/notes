benchmark.hpp
```
struct BenchmarkState {
	virtual ~BenchmarkState() { }
};

class Benchmark {
	constexpr static size_t DEFAULT_NRUNS = 5;
	constexpr static size_t DEFAULT_TIMEOUT = 30;
	Benchmark(Benchmark &) = delete;
public:
	string name;
	string group;

	Benchmark(bool register_benchmark, string name, string group);

	virtual unique_ptr<BenchmarkState> Initialize() { }
	virtual void Run(BenchmarkState *state) = 0;
	virtual void Cleanup(BenchmarkState *state) = 0;
	virtual string Verify(BenchmarkState *state) = 0;
	virtual void Finalize() { } 
	string GetInfo() { }
	virtual string GetLogOutput(BenchmarkState *state) = 0;
 
	virtual bool RequireReinit() { } 
	virtual size_t NRuns() { } 
	virtual size_t Timeout() { }
};
```
包含一个 State 和 接口类 Benchmark。  
接口类有两个成员，表示该 benck 的名字和组类。  
所有的 bench 都继承自该接口。

State 保存了数据库的信息。详见后面 DuckDBBenchmarkState .

>后面有 = 0 ，表示派生类一定要继承，  
>Finalize() { } 就可以不需要重写（实际上也继承了）。

duckdb_benchmark.hpp
```

struct DuckDBBenchmarkState : public BenchmarkState {
	DuckDB db;
	Connection conn;
	unique_ptr<QueryResult> result;

	DuckDBBenchmarkState(string path) : db(path.empty() ? nullptr : path.c_str()), conn(db) { }
	virtual ~DuckDBBenchmarkState() { }
};

class DuckDBBenchmark : public Benchmark {
public:
	DuckDBBenchmark(bool register_benchmark, string name, string group) : Benchmark(register_benchmark, name, group) { }
	virtual ~DuckDBBenchmark() { }

	virtual void Load(DuckDBBenchmarkState *state) = 0;
	virtual string GetQuery() { } 
	virtual void RunBenchmark(DuckDBBenchmarkState *state) { } 
	virtual void Cleanup(DuckDBBenchmarkState *state){};
	virtual string VerifyResult(QueryResult *result) = 0;
	virtual bool InMemory() { }
	string GetDatabasePath() { }

	virtual unique_ptr<DuckDBBenchmarkState> CreateBenchmarkState() {
		return make_unique<DuckDBBenchmarkState>(GetDatabasePath());
	}

	unique_ptr<BenchmarkState> Initialize() override {
		auto state = CreateBenchmarkState();
		Load(state.get());
		return move(state);
	}
	void Run(BenchmarkState *state_) override {	}
	void Cleanup(BenchmarkState *state_) override { }
	string Verify(BenchmarkState *state_) override { }
	string GetLogOutput(BenchmarkState *state_) override { }
	void Interrupt(BenchmarkState *state_) override { }
};

} // namespace duckdb

```
可以看到除了几个带 override 的函数继承自基类，其他几个是 DuckDBBenchmark 自己的，且有的是虚函数。  
继承的几个函数通过传入的派生state_ ,再转成对应派生state实现的。  
>值得注意的是有两个 Cleanup()，但是参数不同，相当于重载。

sqlite_benchmark.hpp 同理


```
class Benchmark  
class DuckDBBenchmark : public Benchmark{}
class SQLiteBenchmark : public Benchmark{}

struct BenchmarkState
struct DuckDBBenchmarkState : public BenchmarkState{}
struct SQLiteBenchmarkState : public BenchmarkState{}
```

有意思的是，sqlite_benchmark.hpp 有对应的 .cpp文件，duckdb_benchmark.hpp却没有。  
DuckDBBenchmark 部分继承自基类的函数已经在 .hpp 实现了，但是 DuckDBBenchmark 有自己的一些函数，且部分虚函数还未实现。