# Projeto Gestor de memória em C

### gc_meumaloco

## Objetivo

* O objetivo com este projeto é estudar e investigar a gestão de memória em linguagens de programação como Java, Python, etc..

* Este objetivo surge da curiosidade em perceber melhor o funcionamento da memória por detrás de todas as abstrações que as linguagens modernas nos fornecem.

* Para este fim, vou estudar como foram implementados garbage collectors noutras linguagens, algoritmos como mark-and-sweep, o funcionamento do heap e do stack.

## Autor, data, contexto do trabalho

_**João Mendes**_

_**Março/Abril 2025**_

_**UF26 – 10790 - Projeto de programação**_

## Descrição do problema

* Em linguagens de programação mais recentes e sofisticadas, existem mecanismos de gerir memória, que, em segundo plano, retiram ao programador a preocupação e responsabilidade de questões triviais como a alocação/realocação e a libertação da memória.

* Desta maneira o programador não tem de estar constantemente a gerir manualmente a memória no seu programa.
Esta tarefa é feita de modo automático e dinâmica pelo Garbage Collector.

* O problema, na minha opinião consiste na grande abstração que existe por detrás destes mecanismos, e a falta de conhecimento fundamental que resulta do comodismo de usar estas facilidades, por vezes fazem com que tomemos estas operações cruciais como garantidas, uma vez que as linguagens tratam destes processos todos.

## Ferramentas e versões

* Linguagem: C

* Compilador: Gcc --version 14.2.1

* Editor de texto: Nvim --version 0.11.0

## Análise de dados

* O projeto trabalhará com estruturas de dados para gerir a memória.

* Serão implementadas estruturas de dados para rastrear para rastrear todos os objetos alocados, para manter referências, raízes, e um sistema de marcação para identificar objetos alcançáveis.  

## Análise de processos

* O projeto será dividido em várias etapas, incluindo a definição da arquitetura, implementação das funcionalidades principais, e a criação de exemplos de uso e documentação.

## Descrição de processos

1. **Definição da Arquitetura:**

    * Estruturar o projeto com múltiplos arquivos para modularidade.

2. **Implementação das Funcionalidades Principais:**

    * Sistema de mark-and-sweep,

    * Alocação de memória,

    * Rastreamento de referências,

    * Libertamento de memória.

## Dicionário

1. **Garbage Collector:**

    * Componente que faz gestão automatica da memória alocada, libertando memória não utilizada.

2. **Mark-and-Sweep:**

    * Algoritmo utilizado para identificar e libertar memória não utilizada.

3. **Raiz:**

    * Objeto diretamente acessível pelo programa.

4. **Referência:**

    * Conexão entre dois objetos, permitindo que um objeto acesse outro.

## String connection, instâncias, bases de dados

* Como o projeto não envolve diretamente bases de dados, não há string connection ou instâncias a serem listadas.

## Lista de tabelas, campos e tipos

* Como o projeto não envolve diretamente bases de dados, não há tabelas, campos ou tipos a serem listados.

## Namespaces

* O projeto será estruturado em múltiplos ficheiros C, utilizando header files para definir as funções e as estruturas de dados.

* Em C não há namespaces específicos como em C++, mas a modularidade será mantida através da separação de arquivos.

## Classes e métodos

* Em C, vou utilizar estruturas (structs) para definir objetos e funções para métodos. As principais estruturas e funções serão:

1. **Estruturas:**

    `gc_object_t` Estrutura principal do coletor de lixo, guarda os objetos geridos pelo nosso garbage collector

    `gc_stack_t` Estrutura utilizada para guardar objetos dentro de um frame na maquina virtual.

    `gc_vm_t` Estrutura utilizada para contar referencias entre objetos que pertecem ao stack.

2. **Funções:**

    `gc_collect_garbage(gc_vm_t *vm)`

    `gc_trace(gc_vm_t *vm)`

    `gc_mark(gc_vm_t *vm)`

    `gc_sweep(gc_vm_t *vm)`
