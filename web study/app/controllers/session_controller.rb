class SessionController < ApplicationController
	def new
	end

	def create
		user = User.find_by(username: params[:session][:username].downcase)
		p user
		if (user && user.authenticate(params[:session][:password]))
			log_in user
			redirect_to user
		else
			render 'new'
		end
	end

    def destroy
		session.delete(:user_id)
    @current_user = nil
		redirect_to '/login'
	end
	
 #  def log_out
 #    session.delete(:user_id)
 #    @current_user = nil
	# 	redirect_to '/login'
	# end
end
