<?php

class CS_Controller extends CI_CONTROLLER {

	public $is_logged_in = FALSE;
	public $user = NULL;
	public $is_professor = FALSE;
	public $is_gestor = FALSE;
	public $data = array();

	public function __construct() {
		parent::__construct();

		$this->load->model('user_model');
		$this->get_credentials();

		$this->data['Current_user'] = $this->user;
		$this->data['topbar'] = "topbar_user";
	}
	

	///////////////////////////////////////////////////
	private function get_credentials() {

		$unid = $this->session->userdata('unid');

		if( !empty($unid) ) {

			$this->user = $this->user_model->get_user_from_unid($unid);

			if($this->user !== NULL) {
				$this->is_logged_in = TRUE;
				$this->is_professor = $this->user_model->is_user_professor($this->user['id']);
				$this->is_gestor = $this->user_model->is_user_gestor($this->user['id']);
			} else {
				$this->is_logged_in = FALSE;
			}


		} else {
			$this->is_logged_in = FALSE;
		}
	}

	// @return nothing
	public function log_out() {

		if($this->is_logged_in) {

			$this->session->unset_userdata('unid');

		}

	}

}

class Another one {

	public function arroz() {

	}

	// @return ARRAY OF IDS
	public function linux() {

	}

}
