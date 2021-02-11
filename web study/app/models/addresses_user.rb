class AddressesUser < ActiveRecord::Base
  has_and_belongs_to_many :user
  has_and_belongs_to_many :address
  validates :user, :presence => true
  validates :address, :presence => true
end
