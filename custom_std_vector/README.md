One caveat: this isn't a production-complete std::vector replacement. I will be tackling full allocator semantics, exepction safety, iterator invalidation rules, perfect forwarding, etc later.


The scope of this V1 is: 
- templates
- const correctness
- Rule of Five
- RAII
- raw storage
- object lifetime