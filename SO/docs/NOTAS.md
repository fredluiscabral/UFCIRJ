# Notas didáticas e de compilação

- A seção sobre produtor-consumidor aborda sobrescrita por buffer cheio, leituras sem itens disponíveis e disputa por índices em cenários com múltiplos produtores.
- O material também cobre região crítica, espera ocupada, alternância estrita, Peterson e um panorama prático de OpenMP.
- Pseudocódigos de algoritmos clássicos não devem ser copiados literalmente como sincronização válida em C moderno sem as hipóteses necessárias de atomicidade e ordenação da memória.
- `critical`, `atomic` e `reduction` têm semânticas diferentes. Em particular, `reduction` não é simplesmente outro lock.
- Os três exemplos de produtor-consumidor mostram etapas distintas do desenvolvimento didático; não são versões corretas de produção.
- O Beamer original apresenta alguns avisos de conteúdo vertical excedente (overfull vbox); eles não impedem a compilação, mas merecem revisão visual antes de projetar todos os slides.
