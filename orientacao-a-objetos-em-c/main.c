#include <stdio.h>

struct veiculo {
    // Qualquer atributo que queira na classe
    int rodas;
    float kilometragem;
    float combustivel; // Litros de combustível
    int _estado;
    float _eficiencia; // km/L

    // Agora definimos os métodos da nossa classe
    int (* ligar)(struct veiculo *self);
    int (* desligar)(struct veiculo *self);
    float (* locomover)(struct veiculo *self, float km);
};

/*
Vamos criar um enum pra representar os estados
do veiculo.
*/
enum veiculo_estado {
    VEICULO_DESLIGADO = 1,
    VEICULO_LIGADO,
};

/*
A função de ligar irá mudar o estado do
veiculo se tiver combustível para isso.
*/
int veiculo_ligar(struct veiculo *self) {
    if (self->combustivel == 0) {
        return -1;
    }

    self->_estado = VEICULO_LIGADO;
    return 0;
}

/*
A função de desligar apenas muda o estado do
veiculo.
*/
int veiculo_desligar(struct veiculo *self) {
    self->_estado = VEICULO_DESLIGADO;
    return 0;
}

/*
O método locomover irá aumentar na kilometragem a
quantidade de km andada pelo veiculo dado a quantidade
de combustível presente e sua eficiência, retornando
a quantidade de km andados.
*/
float veiculo_locomover(struct veiculo *self, float km) {
    if (self->_estado == VEICULO_DESLIGADO) {
        return 0;
    }

    float km_max = self->_eficiencia * self->combustivel;

    if (km_max > km) {
        self->kilometragem += km;
        self->combustivel -= km / self->_eficiencia;
        return km;
    }

    self->kilometragem += km_max;
    self->combustivel = 0;
    return km_max;
}

void veiculo_init(struct veiculo *obj, float eficiencia) {
    /*
    Como no C não conseguimos definir os valores padrões
    que serão utilizados na struct, então precisamos
    inicializar todos os atributos com seu valor padrão.
    */
    obj->rodas = 0;
    obj->kilometragem = 0;
    obj->combustivel = 0;
    obj->_estado = VEICULO_DESLIGADO;
    obj->_eficiencia = eficiencia;

    /*
    Vamos atribuir as funções que definimos previamente
    como métodos da nossa classe utilizando os ponteiros
    de funções.
    */
    obj->ligar = veiculo_ligar;
    obj->desligar = veiculo_desligar;
    obj->locomover = veiculo_locomover;

    return;
}

struct carro {
    struct veiculo parent;
};

void carro_init(struct carro *obj, float eficiencia) {
    veiculo_init(&obj->parent, eficiencia);

    obj->parent.rodas = 4;

    return;
}

struct moto {
    struct veiculo parent;
};

void moto_init(struct moto *obj, float eficiencia) {
    veiculo_init(&obj->parent, eficiencia);

    obj->parent.rodas = 2;

    return;
}

int main(void) {
    struct carro vw_gol;
    struct veiculo *vw_gol_veiculo;

    carro_init(&vw_gol, 10);
    vw_gol_veiculo = (struct veiculo *) &vw_gol;

    // Vamos abastecer o carro
    vw_gol_veiculo->combustivel += 10;

    // Vamos ligar o carro
    vw_gol_veiculo->ligar(vw_gol_veiculo);

    float km_andados = vw_gol_veiculo->locomover(vw_gol_veiculo, 50);
    printf("Andamos %.2fKm!\n\n", km_andados);
    printf("Kilometragem: %.2fKm\n", vw_gol_veiculo->kilometragem);
    printf("Combustivel: %.2fL\n", vw_gol_veiculo->combustivel);
}
