/*
 * BOOOS.h
 *
 *  Created on: Aug 14, 2014
 *      Author:Ana Luiza & Karoline
 */

#include <iostream>
#include <stdlib.h>
#include "Queue.h"


using namespace std;
namespace BOOOS {


// Queue constructor: must initialize queue's attributes
Queue::Queue() {

	this->_length=0;
}

// Queue destructor: must finalize queue's attributes
Queue::~Queue() {

	while(this->_length != 0){
		this->remove();

	}
}

// insert method: must insert the Element at the end of the queue
// Error messages:
//   01: invalid Element (if elem == 0)
//   02: duplicated Element (if elem already is on the queue)
void Queue::insert(Queue::Element * elem) {

	Element *atual = new Element();
	atual= this->_head.next();

	if (elem == 0){
		cout << "ERRO 01: invalid Element " << endl;
		return; // se houve falha ao criar um nodo (ex: sem memoria)
	}

	if( this->_length == 0){/* Teste para saber se a lista está vazia*/

		this->_head.next(elem);//this->head()->next(elem);
		this->_head.prev(elem);
		elem->next(this->_head.next());// aponta para o next dele(o segundo) ou aponta pra ele mesmos??
		elem->prev(this->_head.prev());


	}else{ /* Quando a lista não está vazia, adiciona elemento no final*/

		//teste para ver se já existe o elemento na lista
		for(int x= this->_length;x>0;x--){

			if(atual== elem){
				cout << " 02: Elemento Duplicado" << endl;
				return;
			}
			atual= atual->next();
		}

		this->_head.prev()->next(elem);
		elem->prev(this->_head.prev());
		this->_head.prev(elem);
		this->_head.prev()->next(this->_head.next());
		this->_head.next()->prev(this->_head.prev());

	}

	this->_length++;

}


void Queue::insert_ordered(Element * elem){

	Element *atual = new Element();
	Element *aux = new Element();
	aux= this->_head.next();


	if (elem == 0){
		cout << "ERRO 01: invalid Element " << endl;
		return; // se houve falha ao criar um nodo (ex: sem memoria)
	}

	if(this->length() == 0){ // Se não tiver nenhum elemento na lista
		this->insert(elem);
		return;
	}

	for(int x= this->_length;x>0;x--){

		if(aux== elem){
			cout << " 02: Elemento Duplicado" << endl;
			return;
		}
		aux= aux->next();
	}

	if(this->length()==1){
		if(elem->rank()<this->head()->next()->rank()){
			elem->next(this->head()->next());
			elem->prev(this->head()->next());
			this->head()->next()->next(elem);
			this->head()->next()->prev(elem);
			this->head()->next(elem);
			this->head()->prev(this->head()->next()->prev());
			this->_length++;
			return;

		}else{
			this->insert(elem);
			return;
		}
	}

	//inserir como primeiro:
	if (elem->rank()< this->head()->next()->rank()){
		elem->next(this->head()->next());

		elem->prev(this->head()->prev());
		this->head()->next()->prev(elem);
		this->head()->next(elem);
		this->_length++;
		return;
	}
	// inserir no fim:
	if(elem->rank()>=this->head()->prev()->rank()){
		this->insert(elem);
		return;
	}

	for(atual= this->head()->next();atual->rank()<=elem->rank(); atual = atual->next());
	elem->next(atual);
	elem->prev(atual->prev());
	atual->prev()->next(elem);
	atual->prev(elem);
	this->_length++;
	return;

}

// remove: must search and remove the element from the queue
// Error messages:
//   01: queue is empty
Queue::Element * Queue::remove() {


	if (this->_length >0){

		Element *aux = new Element();
		aux = this->_head.next();

		if(this->_length == 1){
			this->_head.next(0);
			this->_head.prev(0);
			this->_length--;
			aux->next(0);
			aux->prev(0);
			return aux;

		}else{
			this->_head.next(this->_head.next()->next());
			this->_head.next()->prev(this->_head.prev());
			this->_head.prev()->next(this->_head.next());
			this->_length--;
			aux->next(0);
			aux->prev(0);
			return aux;
		}

	}
	cout << " ERRO: 01, Lista Vazia! " << endl;
	return NULL;

}
// não testamos!!
Queue::Element * Queue::search(Element * elem) {

	Element *comp = new Element();
	comp= this->_head.next();

	for(int x= this->_length; x > 0;x--){

		if(comp == elem){
//			cout << " Elemento Encontrado, procura" << endl;
			return elem;
		}
		comp= comp->next();
	}
	cout << " Elemento Não Encontrado" << endl;
	return 0;
}

void Queue::remove(Element * elem){

	Element *aux = new Element();

	aux= this->search(elem);
	if (aux ==  0){
		return;
	}
	if(aux == this->_head.next()){
		this->remove();
		return;
	}
	if(aux == this->_head.prev()){
		this->_head.prev(this->_head.prev()->prev());
		this->_head.next()->prev(this->_head.prev());
		this->_head.prev()->next(this->_head.next());
		this->_length--;
		aux->next(0);
		aux->prev(0);
		return;

	}else{
		aux->prev()->next(aux->next());
		aux->next()->prev(aux->prev());
		this->_length--;
		aux->next(0);
		aux->prev(0);
		return;
	}




}
}
/*void Queue:: imprimir(){
	Element *atual = new Element();

	for(atual= this->head()->next(); atual!= this->head()->next(); atual = atual->next()){

	}

}*/
