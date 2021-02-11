class User < ActiveRecord::Base
	def authenticate(p)
		return password ==  p
	end

	def obfuscate(p)
		return "●●●●●●"
	end

	has_one  :profession, dependent: :destroy
	has_many :email, dependent: :destroy
	has_and_belongs_to_many :addresses, dependent: :destroy
end
